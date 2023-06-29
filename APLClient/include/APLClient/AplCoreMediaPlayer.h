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

#ifndef APLCLIENT_APL_APLCOREMEDIAPLAYER_H
#define APLCLIENT_APL_APLCOREMEDIAPLAYER_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#pragma push_macro("DEBUG")
#pragma push_macro("TRUE")
#pragma push_macro("FALSE")
#undef DEBUG
#undef TRUE
#undef FALSE
#include <apl/apl.h>
#pragma pop_macro("DEBUG")
#pragma pop_macro("TRUE")
#pragma pop_macro("FALSE")
#pragma GCC diagnostic pop

#include "AplConfiguration.h"
#include "AplCoreConnectionManager.h"

namespace APLClient {

class AplCoreMediaPlayer;

using AplCoreMediaPlayerPtr = std::shared_ptr<AplCoreMediaPlayer>;

/**
 * MediaPlayer.
 */
class AplCoreMediaPlayer : public apl::MediaPlayer {
public:
    static AplCoreMediaPlayerPtr create(AplCoreConnectionManagerWPtr aplCoreConnectionManager,
                                        AplConfigurationPtr config,
                                        const std::string& playerId,
                                        apl::MediaPlayerCallback&& playerCallback);

    AplCoreMediaPlayer(AplCoreConnectionManagerWPtr aplCoreConnectionManager,
                       AplConfigurationPtr config,
                       const std::string& playerId,
                       apl::MediaPlayerCallback&& playerCallback);
    ~AplCoreMediaPlayer() override = default;

    void updateMediaState(const rapidjson::Value& payload);
    void doCallback(const rapidjson::Value& payload);
    std::string getPlayerId() const { return m_playerId; };

    // apl::MediaPlayer overrides
    void release() override;
    void halt() override;
    void setTrackList(std::vector<apl::MediaTrack> tracks) override;
    void play(apl::ActionRef actionRef) override;
    void pause() override;
    void next() override;
    void previous() override;
    void rewind() override;
    void seek(int offset) override;
    void seekTo(int position) override;
    void setTrackIndex(int trackIndex) override;
    void setAudioTrack(apl::AudioTrack audioTrack) override;
    void setMute(bool mute) override;
    rapidjson::Value serialize(rapidjson::Document::AllocatorType& allocator) const override;

private:
    void resolveExistingAction();
    bool isActive() const;
    void sendMediaPlayerCommand(const std::string& command, rapidjson::Value&& payload);

private:
    AplCoreConnectionManagerWPtr m_aplCoreConnectionManager;
    AplConfigurationPtr m_aplConfiguration;
    
    std::string m_playerId;
    apl::AudioTrack m_audioTrack;
    apl::ActionRef m_actionRef = apl::ActionRef(nullptr);
    bool m_released = false; // Set when the media player is released and should not be used
    bool m_halted = false;   // Set when the media player was asked to halt all playback
    apl::MediaState m_mediaState;
};

using AplCoreMediaPlayerPtr = std::shared_ptr<AplCoreMediaPlayer>;

} // namespace APLClient

#endif // APLCLIENT_APL_APLCOREMEDIAPLAYER_H
