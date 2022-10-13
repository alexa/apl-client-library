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

#ifndef APLCLIENT_APL_APLCOREAUDIOPLAYER_H
#define APLCLIENT_APL_APLCOREAUDIOPLAYER_H

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

/**
 * AudioPlayer adapter. Controls browser-owned player.
 */
class AplCoreAudioPlayer : public apl::AudioPlayer {
public:
    /**
     * Constructor
     *
     * @param aplCoreConnectionManager Pointer to the APL Core connection manager
     * @param config Pointer to APL configuration
     * @param playerId PLayer ID.
     * @param playerCallback Player events callback.
     * @param speechMarkCallback TTS speechmarks callback.
     */
    AplCoreAudioPlayer(
            AplCoreConnectionManagerWPtr aplCoreConnectionManager,
            AplConfigurationPtr config,
            const std::string& playerId,
            apl::AudioPlayerCallback&& playerCallback,
            apl::SpeechMarkCallback&& speechMarkCallback);

    /**
     * @param aplCoreConnectionManager Pointer to the APL Core connection manager
     * @param config Pointer to APL configuration
     * @param playerId PLayer ID.
     * @param playerCallback Player events callback.
     * @param speechMarkCallback TTS speechmarks callback.
     */
    static std::shared_ptr<AplCoreAudioPlayer> create(
            AplCoreConnectionManagerWPtr aplCoreConnectionManager,
            AplConfigurationPtr config,
            const std::string& playerId,
            apl::AudioPlayerCallback&& playerCallback,
            apl::SpeechMarkCallback&& speechMarkCallback);

    /// @name apl::AudioPlayer Functions
    /// @{
    void release() override;
    void setTrack(apl::MediaTrack track) override;
    void play(apl::ActionRef actionRef) override;
    void pause() override;
    /// @}

    /**
     * Process AudioPlayer event received from the browser.
     * @param payload message payload.
     */
    void onEvent(const rapidjson::Value& payload);

    /**
     * Process SpeechMarks received from the browser.
     * @param payload message payload.
     */
    void onSpeechMarks(const rapidjson::Value& payload);

    /**
     * Simulate time update.
     *
     * @param connectionManager AplCoreConnectionManager
     */
    void tick(const AplCoreConnectionManager& connectionManager);

private:
    void sendAudioPlayerCommand(const std::string& command, std::string optionalUrl = "");
    void resolveExistingAction();
    void onEventInternal(apl::AudioPlayerEventType eventType, const apl::AudioState& audioState, int currentTime);

private:
    AplCoreConnectionManagerWPtr m_aplCoreConnectionManager;
    AplConfigurationPtr m_aplConfiguration;

    apl::ActionRef m_PlayRef = apl::ActionRef(nullptr);
    bool m_Playing = false;
    bool m_Prepared = false;
    int m_PlaybackStartTime = 0;
    std::string m_playerId;
};

using AplCoreAudioPlayerPtr = std::shared_ptr<AplCoreAudioPlayer>;

}  // namespace APLClient

#endif  // APLCLIENT_APL_APLCOREAUDIOPLAYER_H
