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
#include "APLClient/AplCoreAudioPlayerFactory.h"
#include "APLClient/AplCoreAudioPlayer.h"

namespace APLClient {

AplCoreAudioPlayerFactory::AplCoreAudioPlayerFactory(
        AplCoreConnectionManagerPtr aplCoreConnectionManager,
        AplConfigurationPtr config) :
    m_aplCoreConnectionManager{aplCoreConnectionManager},
    m_aplConfiguration{config} {
}

AplCoreAudioPlayerFactoryPtr
AplCoreAudioPlayerFactory::create(AplCoreConnectionManagerPtr aplCoreConnectionManager, AplConfigurationPtr config) {
    return std::make_shared<AplCoreAudioPlayerFactory>(aplCoreConnectionManager, config);
}

apl::AudioPlayerPtr
AplCoreAudioPlayerFactory::createPlayer(apl::AudioPlayerCallback playerCallback,
                                        apl::SpeechMarkCallback speechMarkCallback) {
    // Only one player really supported
    std::string id = "WEB_PLAYER_ADAPTER";
    m_Players[id] = AplCoreAudioPlayer::create(
            m_aplCoreConnectionManager,
            m_aplConfiguration,
            id,
            std::move(playerCallback),
            std::move(speechMarkCallback));
    return m_Players[id];
}

AplCoreAudioPlayerPtr
AplCoreAudioPlayerFactory::getPlayer(const std::string& playerId) const {
    auto player = m_Players.find(playerId);
    if (player == m_Players.end()) {
        return nullptr;
    }
    return player->second;
}

void
AplCoreAudioPlayerFactory::tick(const AplCoreConnectionManager& connectionManager) {
    for (auto& player : m_Players) {
        player.second->tick(connectionManager);
    }
}


} // namespace APLClient
