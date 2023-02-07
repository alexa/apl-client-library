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
#include "APLClient/AplCoreAudioPlayer.h"

namespace APLClient {

AplCoreAudioPlayerPtr
AplCoreAudioPlayer::create(
        AplCoreConnectionManagerWPtr aplCoreConnectionManager,
        AplConfigurationPtr config,
        const std::string& playerId,
        apl::AudioPlayerCallback&& playerCallback,
        apl::SpeechMarkCallback&& speechMarkCallback) {
    if (auto connectionManager = aplCoreConnectionManager.lock()) {
        auto msg = AplCoreViewhostMessage("createAudioPlayer");
        auto& alloc = msg.alloc();

        auto aplCoreMetrics = connectionManager->aplCoreMetrics();

        rapidjson::Value payload(rapidjson::kObjectType);
        payload.AddMember("playerId", rapidjson::Value(playerId, alloc).Move(), alloc);
        msg.setPayload(std::move(payload));

        connectionManager->blockingSend(msg);
    } else {
        auto aplOptions = config->getAplOptions();
        aplOptions->logMessage(LogLevel::WARN, __func__, "ConnectionManager does not exist. Can't create AudioPlayer.");
        return nullptr;
    }

    return std::make_shared<AplCoreAudioPlayer>(
            aplCoreConnectionManager,
            config,
            playerId,
            std::move(playerCallback),
            std::move(speechMarkCallback));
}

AplCoreAudioPlayer::AplCoreAudioPlayer(
        AplCoreConnectionManagerWPtr aplCoreConnectionManager,
        AplConfigurationPtr config,
        const std::string& playerId,
        apl::AudioPlayerCallback&& playerCallback,
        apl::SpeechMarkCallback&& speechMarkCallback) :
    apl::AudioPlayer(std::move(playerCallback), std::move(speechMarkCallback)),
    m_aplCoreConnectionManager{aplCoreConnectionManager},
    m_aplConfiguration{config},
    m_playerId{playerId} {
}

void
AplCoreAudioPlayer::sendAudioPlayerCommand(const std::string& command, std::string optionalUrl) {
    if (auto connectionManager = m_aplCoreConnectionManager.lock()) {
        auto msg = AplCoreViewhostMessage(command);
        auto& alloc = msg.alloc();

        auto aplCoreMetrics = connectionManager->aplCoreMetrics();

        rapidjson::Value payload(rapidjson::kObjectType);
        payload.AddMember("playerId", rapidjson::Value(m_playerId.c_str(), alloc).Move(), alloc);
        if (!optionalUrl.empty()) {
            payload.AddMember("url", rapidjson::Value(optionalUrl.c_str(), alloc).Move(), alloc);
        }
        msg.setPayload(std::move(payload));

        connectionManager->send(msg);
    } else {
        auto aplOptions = m_aplConfiguration->getAplOptions();
        aplOptions->logMessage(LogLevel::WARN, __func__, "ConnectionManager does not exist. Can't send AudioPlayer command.");
    }
}

void
AplCoreAudioPlayer::resolveExistingAction()
{
    m_Playing = false;
    if (!m_PlayRef.empty() && m_PlayRef.isPending()) {
        m_PlayRef.resolve();
    }
    m_PlayRef = apl::ActionRef(nullptr);
}

void
AplCoreAudioPlayer::release() {
    sendAudioPlayerCommand("audioPlayerRelease");
    resolveExistingAction();
}

void
AplCoreAudioPlayer::setTrack(apl::MediaTrack track) {
    sendAudioPlayerCommand("audioPlayerSetTrack", track.url);
}

void
AplCoreAudioPlayer::play(apl::ActionRef actionRef) {
    resolveExistingAction();

    m_PlayRef = actionRef;
    if (m_Prepared) {
        sendAudioPlayerCommand("audioPlayerPlay");
    }
    if (m_Failed) {
        resolveExistingAction();
    }
}

void
AplCoreAudioPlayer::pause() {
    sendAudioPlayerCommand("audioPlayerPause");
    resolveExistingAction();
}

void
AplCoreAudioPlayer::onEventInternal(apl::AudioPlayerEventType eventType, const apl::AudioState& audioState, int currentTime) {
    switch (eventType) {
        case apl::kAudioPlayerEventEnd:
        case apl::kAudioPlayerEventFail:
            m_Failed = true;
            resolveExistingAction();
            break;
        case apl::kAudioPlayerEventReady:
            m_Prepared = true;
            if (!m_PlayRef.empty()) {
                // Requested playback before prepared. Do now.
                sendAudioPlayerCommand("audioPlayerPlay");
            }
            break;
        case apl::kAudioPlayerEventPlay:
            m_Playing = true;
            m_PlaybackStartTime = currentTime;
            break;
        case apl::kAudioPlayerEventTimeUpdate:
            if (!m_Playing) return;
            break;
        default:
            break;
    }

    mPlayerCallback(eventType, audioState);
}

void
AplCoreAudioPlayer::onEvent(const rapidjson::Value& payload) {
    auto aplOptions = m_aplConfiguration->getAplOptions();
    auto connectionManager = m_aplCoreConnectionManager.lock();

    if (!connectionManager) {
        aplOptions->logMessage(LogLevel::WARN, __func__, "ConnectionManager does not exist.");
        return;
    }

    auto currentTime = static_cast<int>(connectionManager->getCurrentTime().count());
    auto eventType = static_cast<apl::AudioPlayerEventType>(payload["eventType"].GetInt());
    auto audioState = apl::AudioState(
            m_Playing ? currentTime - m_PlaybackStartTime : 0,
            payload["duration"].GetDouble(),
            payload["paused"].GetBool(),
            payload["ended"].GetBool(),
            static_cast<apl::TrackState>(payload["trackState"].GetDouble())
    );

    onEventInternal(eventType, audioState, currentTime);
}

static const std::map<std::string, apl::SpeechMarkType> sSpeechTypes = {
    {"word", apl::SpeechMarkType::kSpeechMarkWord},
    {"viseme", apl::SpeechMarkType::kSpeechMarkViseme},
    {"sentence", apl::SpeechMarkType::kSpeechMarkSentence},
    {"ssml", apl::SpeechMarkType::kSpeechMarkSSML}
};

void
AplCoreAudioPlayer::onSpeechMarks(const rapidjson::Value& payload) {
    if (!mSpeechMarkCallback) return;

    auto marks = std::vector<apl::SpeechMark>();
    for (auto itr = payload["markers"].Begin(); itr != payload["markers"].End(); ++itr) {
        auto type = sSpeechTypes.at((*itr)["type"].GetString());
        apl::SpeechMark mark = {
                .type = type,
                .start = 0,
                .end = 0,
                .time = static_cast<unsigned long>((*itr)["time"].GetDouble()),
                .value = (*itr)["value"].GetString()
        };

        if (type == apl::kSpeechMarkWord) {
            mark.start = (*itr)["start"].GetDouble();
            mark.end = (*itr)["end"].GetDouble();
        }
        marks.emplace_back(mark);
    }

    mSpeechMarkCallback(marks);
}

void
AplCoreAudioPlayer::tick(const AplCoreConnectionManager& connectionManager) {
    if (m_Playing) {
        auto state = apl::AudioState(connectionManager.getCurrentTime().count() - m_PlaybackStartTime, 0, false, false, apl::kTrackReady);
        onEventInternal(apl::kAudioPlayerEventTimeUpdate, state, connectionManager.getCurrentTime().count());
    }
}

} // namespace APLClient
