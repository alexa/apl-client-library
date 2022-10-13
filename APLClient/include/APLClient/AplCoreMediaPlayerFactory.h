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

#ifndef APLCLIENT_APL_APLCOREMEDIAPLAYERFACTORY_H
#define APLCLIENT_APL_APLCOREMEDIAPLAYERFACTORY_H

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

#include <cstddef>

#include "AplConfiguration.h"
#include "AplCoreConnectionManager.h"
#include "AplCoreMediaPlayer.h"

namespace APLClient {

/**
 * MediaPlayer factory.
 */
class AplCoreMediaPlayerFactory : public apl::MediaPlayerFactory {
public:
    AplCoreMediaPlayerFactory(AplCoreConnectionManagerWPtr aplCoreConnectionManager, AplConfigurationPtr config);

    static std::shared_ptr<AplCoreMediaPlayerFactory> create(AplCoreConnectionManagerWPtr aplCoreConnectionManager, AplConfigurationPtr config);

    AplCoreMediaPlayerPtr getMediaPlayer(const std::string& playerId) const;

    // apl::MediaPlayerFactory overrides
    apl::MediaPlayerPtr createPlayer(apl::MediaPlayerCallback callback) override;

private:
    /**
     * Removes old inactive players from the list of players, if possible.
     */
    void cleanup();

    /**
     * Delete media player on the other side of the web socket
     */
    void sendMediaPlayerDelete(const std::string& mediaPlayerId);

private:
    AplCoreConnectionManagerWPtr m_aplCoreConnectionManager;
    AplConfigurationPtr m_aplConfiguration;
    std::map<std::string, std::weak_ptr<AplCoreMediaPlayer>> m_activePlayers;
};

} // namespace APLClient

#endif // APLCLIENT_APL_APLCOREMEDIAPLAYERFACTORY_H
