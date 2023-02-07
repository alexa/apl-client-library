/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include "APLClient/AplCoreViewhostMessage.h"
#include "APLClient/AplCoreMediaPlayer.h"

namespace APLClient {

static std::string textTrackTypeToKind(apl::TextTrackType type) {
    switch (type) {
        case apl::TextTrackType::kTextTrackTypeCaption:
            return "captions";
        default:
            return "unsupported";
    }
}

std::shared_ptr<AplCoreMediaPlayer>
AplCoreMediaPlayer::create(AplCoreConnectionManagerWPtr aplCoreConnectionManager,
                           AplConfigurationPtr config,
                           const std::string& playerId,
                           apl::MediaPlayerCallback&& playerCallback) {
    auto player = std::make_shared<AplCoreMediaPlayer>(aplCoreConnectionManager, config, playerId, std::move(playerCallback));
    return player;
}

AplCoreMediaPlayer::AplCoreMediaPlayer(AplCoreConnectionManagerWPtr aplCoreConnectionManager,
                                       AplConfigurationPtr config,
                                       const std::string& playerId,
                                       apl::MediaPlayerCallback&& playerCallback)
    : apl::MediaPlayer(std::move(playerCallback)),
    m_aplCoreConnectionManager(aplCoreConnectionManager),
    m_aplConfiguration(config),
    m_playerId(playerId)
{
    if (auto connectionManager = m_aplCoreConnectionManager.lock()) {
        auto msg = AplCoreViewhostMessage("mediaPlayerCreate");
        auto& alloc = msg.alloc();

        rapidjson::Value payload(rapidjson::kObjectType);
        payload.AddMember("playerId", rapidjson::Value(m_playerId.c_str(), alloc).Move(), alloc);
        msg.setPayload(std::move(payload));

        connectionManager->blockingSend(msg);
    } else {
        auto aplOptions = m_aplConfiguration->getAplOptions();
        aplOptions->logMessage(LogLevel::WARN, __func__, "ConnectionManager does not exist. Can't send mediaPlayerCreate");
    }
}

void
AplCoreMediaPlayer::resolveExistingAction()
{
    if (!m_actionRef.empty() && m_actionRef.isPending()) {
        m_actionRef.resolve();
    }
    m_actionRef = apl::ActionRef(nullptr);
}

void
AplCoreMediaPlayer::sendMediaPlayerCommand(const std::string& command, rapidjson::Value&& payload)
{
    if (auto connectionManager = m_aplCoreConnectionManager.lock()) {
        auto msg = AplCoreViewhostMessage(command);
        auto& alloc = msg.alloc();

        payload.AddMember("playerId", rapidjson::Value(m_playerId.c_str(), alloc).Move(), alloc);
        msg.setPayload(std::move(payload));

        connectionManager->send(msg);
    } else {
        auto aplOptions = m_aplConfiguration->getAplOptions();
        aplOptions->logMessage(LogLevel::WARN, __func__, "ConnectionManager does not exist. Can't send command: " + command);
    }
}

void
AplCoreMediaPlayer::release()
{
    resolveExistingAction();
    m_released = true;
}

void
AplCoreMediaPlayer::halt()
{
    if (!isActive()) return;
    resolveExistingAction();

    m_halted = true;
    sendMediaPlayerCommand("mediaPlayerStop", rapidjson::Value(rapidjson::kObjectType));
}

void
AplCoreMediaPlayer::setTrackList(std::vector<apl::MediaTrack> tracks)
{
    if (!isActive()) return;
    resolveExistingAction();

    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> alloc;
    rapidjson::Value payload(rapidjson::kObjectType);
    rapidjson::Value trackArray(rapidjson::kArrayType);

    for (auto& track : tracks) {
        rapidjson::Value trackObj(rapidjson::kObjectType);
        trackObj.AddMember("url", track.url, alloc);
        trackObj.AddMember("offset", track.offset, alloc);
        trackObj.AddMember("duration", track.duration, alloc);
        trackObj.AddMember("repeatCount", track.repeatCount, alloc);

        rapidjson::Value textTrackArray(rapidjson::kArrayType);
        for (auto& textTrack : track.textTracks) {
            rapidjson::Value textTrackObj(rapidjson::kObjectType);
            textTrackObj.AddMember("kind", textTrackTypeToKind(textTrack.type), alloc);
            textTrackObj.AddMember("url", textTrack.url, alloc);
            textTrackObj.AddMember("description", textTrack.description, alloc);

            textTrackArray.PushBack(textTrackObj, alloc);
        }
        trackObj.AddMember("textTracks", textTrackArray, alloc);

        trackArray.PushBack(trackObj, alloc);
    }

    payload.AddMember("trackArray", trackArray, alloc);
    sendMediaPlayerCommand("mediaPlayerSetTrackList", std::move(payload));
}

void
AplCoreMediaPlayer::play(apl::ActionRef actionRef)
{
    bool waitForFinish = false;

    if (!isActive()) {
        if (!actionRef.empty()) actionRef.resolve();
        return;
    }
    
    resolveExistingAction();

    if (!actionRef.empty()) {
        // Only hold onto the ActionRef in foreground mode
        if (m_audioTrack == apl::kAudioTrackForeground) {
            m_actionRef = actionRef;
            waitForFinish = true;

            // On a termination we need to discard the action reference or there is a memory cycle
            m_actionRef.addTerminateCallback(
                    [&](const apl::TimersPtr&) { m_actionRef = apl::ActionRef(nullptr); });
        } else {
            actionRef.resolve();
        }
    }

    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> alloc;
    rapidjson::Value payload(rapidjson::kObjectType);
    payload.AddMember("waitForFinish", waitForFinish, alloc);
    sendMediaPlayerCommand("mediaPlayerPlay", std::move(payload));
}

void
AplCoreMediaPlayer::pause()
{
    if (!isActive()) return;
    resolveExistingAction();

    sendMediaPlayerCommand("mediaPlayerPause", rapidjson::Value(rapidjson::kObjectType));
}

void
AplCoreMediaPlayer::next()
{
    if (!isActive()) return;
    resolveExistingAction();

    sendMediaPlayerCommand("mediaPlayerNext", rapidjson::Value(rapidjson::kObjectType));
}

void
AplCoreMediaPlayer::previous()
{
    if (!isActive()) return;
    resolveExistingAction();

    sendMediaPlayerCommand("mediaPlayerPrevious", rapidjson::Value(rapidjson::kObjectType));
}

void
AplCoreMediaPlayer::rewind()
{
    if (!isActive()) return;
    resolveExistingAction();

    sendMediaPlayerCommand("mediaPlayerRewind", rapidjson::Value(rapidjson::kObjectType));
}

void
AplCoreMediaPlayer::seek(int offset)
{
    if (!isActive()) return;
    resolveExistingAction();

    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> alloc;
    rapidjson::Value payload(rapidjson::kObjectType);
    payload.AddMember("offset", offset, alloc);
    sendMediaPlayerCommand("mediaPlayerSeek", std::move(payload));
}

void
AplCoreMediaPlayer::setTrackIndex(int trackIndex)
{
    if (!isActive()) return;
    resolveExistingAction();

    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> alloc;
    rapidjson::Value payload(rapidjson::kObjectType);
    payload.AddMember("trackIndex", trackIndex, alloc);
    sendMediaPlayerCommand("mediaPlayerSetTrackIndex", std::move(payload));
}

void
AplCoreMediaPlayer::setAudioTrack(apl::AudioTrack audioTrack)
{
    if (!isActive()) return;

    m_audioTrack = audioTrack;

    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> alloc;
    rapidjson::Value payload(rapidjson::kObjectType);
    payload.AddMember("audioTrack", static_cast<int>(m_audioTrack), alloc);
    sendMediaPlayerCommand("mediaPlayerSetAudioTrack", std::move(payload));
}

void
AplCoreMediaPlayer::setMute(bool mute)
{
    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> alloc;
    rapidjson::Value payload(rapidjson::kObjectType);
    payload.AddMember("mute", mute, alloc);
    sendMediaPlayerCommand("mediaPlayerSetMute", std::move(payload));
}

bool
AplCoreMediaPlayer::isActive() const
{
    return !m_released && !m_halted;
}

void
AplCoreMediaPlayer::updateMediaState(const rapidjson::Value& payload)
{
    bool mutedValue = false;
    auto muted = payload["mediaState"].FindMember("muted");
    if (muted != payload["mediaState"].MemberEnd()) {
        mutedValue = muted->value.Get<bool>();
    }

    apl::MediaState mediaState(payload["mediaState"].FindMember("trackIndex")->value.Get<int>(),
                               payload["mediaState"].FindMember("trackCount")->value.Get<int>(),
                               payload["mediaState"].FindMember("currentTime")->value.Get<double>(),
                               payload["mediaState"].FindMember("duration")->value.Get<double>(),
                               payload["mediaState"].FindMember("paused")->value.Get<bool>(),
                               payload["mediaState"].FindMember("ended")->value.Get<bool>(),
                               mutedValue);

    auto trackState = payload["mediaState"].FindMember("trackState");
    if (trackState != payload["mediaState"].MemberEnd()) {
        mediaState.withTrackState(static_cast<apl::TrackState>(trackState->value.Get<int>()));
    }

    m_mediaState = mediaState;
}

void
AplCoreMediaPlayer::doCallback(const rapidjson::Value& payload)
{
    if (!isActive()) return;

    auto eventType = static_cast<apl::MediaPlayerEventType>(payload["eventType"].GetInt());
    if (eventType == apl::MediaPlayerEventType::kMediaPlayerEventEnd
        || eventType == apl::MediaPlayerEventType::kMediaPlayerEventTrackFail) {
        resolveExistingAction();
    }
    auto callback = mCallback;
    callback(eventType, m_mediaState);
}

rapidjson::Value
AplCoreMediaPlayer::serialize(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value mediaPlayer(rapidjson::kObjectType);
    mediaPlayer.AddMember("mediaPlayerId", rapidjson::Value(m_playerId.c_str(), allocator).Move(), allocator);
    return mediaPlayer;
}

} // namespace APLClient
