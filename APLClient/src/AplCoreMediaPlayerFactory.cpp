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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#pragma push_macro("DEBUG")
#pragma push_macro("TRUE")
#pragma push_macro("FALSE")
#undef DEBUG
#undef TRUE
#undef FALSE
#include <apl/component/videocomponent.h>
#pragma pop_macro("DEBUG")
#pragma pop_macro("TRUE")
#pragma pop_macro("FALSE")
#pragma GCC diagnostic pop

#include "APLClient/AplCoreViewhostMessage.h"
#include "APLClient/AplCoreMediaPlayerFactory.h"
#include "APLClient/AplCoreMediaPlayer.h"

namespace APLClient {

AplCoreMediaPlayerFactory::AplCoreMediaPlayerFactory(AplCoreConnectionManagerWPtr aplCoreConnectionManager, AplConfigurationPtr config)
    : m_aplCoreConnectionManager(aplCoreConnectionManager), m_aplConfiguration(config) {}

AplCoreMediaPlayerFactory::~AplCoreMediaPlayerFactory()
{
    for (auto it = m_activePlayers.begin(); it != m_activePlayers.end(); ++it) {
        sendMediaPlayerDelete(it->first);
    }
}

std::shared_ptr<AplCoreMediaPlayerFactory>
AplCoreMediaPlayerFactory::create(AplCoreConnectionManagerWPtr aplCoreConnectionManager, AplConfigurationPtr config)
{
    return std::make_shared<AplCoreMediaPlayerFactory>(aplCoreConnectionManager, config);
}

AplCoreMediaPlayerPtr
AplCoreMediaPlayerFactory::getMediaPlayer(const std::string& playerId) const
{
    auto mediaPlayer = m_activePlayers.find(playerId);
    if (mediaPlayer == m_activePlayers.end()) {
        return nullptr;
    } else {
        return mediaPlayer->second.lock();
    }
}

apl::MediaPlayerPtr
AplCoreMediaPlayerFactory::createPlayer(apl::MediaPlayerCallback callback)
{
    cleanup();

    static unsigned int id = 0;
    std::string playerId = std::to_string(++id);
    auto mediaPlayer = AplCoreMediaPlayer::create(
        m_aplCoreConnectionManager,
        m_aplConfiguration,
        playerId,
        std::move(callback)
    );

    m_activePlayers.emplace(playerId, mediaPlayer);

    return mediaPlayer;
}

void
AplCoreMediaPlayerFactory::cleanup()
{
    for (auto it = m_activePlayers.begin(); it != m_activePlayers.end(); ) {
        if (!it->second.lock()) {
            // weak pointer is no longer valid, prune it from the array
            sendMediaPlayerDelete(it->first);
            it = m_activePlayers.erase(it);
        } else {
            ++it;
        }
    }
}

void
AplCoreMediaPlayerFactory::sendMediaPlayerDelete(const std::string& mediaPlayerId)
{
    if (auto connectionManager = m_aplCoreConnectionManager.lock()) {
        auto msg = AplCoreViewhostMessage("mediaPlayerDelete");
        auto& alloc = msg.alloc();

        rapidjson::Value payload(rapidjson::kObjectType);
        payload.AddMember("playerId", rapidjson::Value(mediaPlayerId.c_str(), alloc).Move(), alloc);
        msg.setPayload(std::move(payload));

        connectionManager->send(msg);
    } else {
        auto aplOptions = m_aplConfiguration->getAplOptions();
        aplOptions->logMessage(LogLevel::WARN, __func__, "ConnectionManager does not exist. Can't send mediaPlayerDelete");
    }
}

} // namespace APLClient
