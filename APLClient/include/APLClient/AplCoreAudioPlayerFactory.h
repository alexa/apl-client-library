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

#ifndef APLCLIENT_APL_APLCOREAUDIOPLAYERFACTORY_H
#define APLCLIENT_APL_APLCOREAUDIOPLAYERFACTORY_H

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
#include "AplCoreAudioPlayer.h"

namespace APLClient {

/**
 * AudioPlayer factory.
 */
class AplCoreAudioPlayerFactory : public apl::AudioPlayerFactory {
public:
    /**
     * Constructor
     *
     * @param aplCoreConnectionManager Pointer to the APL Core connection manager
     * @param config Pointer to APL configuration
     */
    AplCoreAudioPlayerFactory(AplCoreConnectionManagerPtr aplCoreConnectionManager, AplConfigurationPtr config);

    /**
     * @param aplCoreConnectionManager Pointer to the APL Core connection manager
     * @param config Pointer to APL configuration
     */
    static std::shared_ptr<AplCoreAudioPlayerFactory> create(AplCoreConnectionManagerPtr aplCoreConnectionManager, AplConfigurationPtr config);

    /**
     * @param playerId Player ID
     * @return Corresponding player.
     */
    AplCoreAudioPlayerPtr getPlayer(const std::string& playerId) const;

    void tick(const AplCoreConnectionManager& connectionManager);

    /// @name apl::AudioPlayerFactory Functions
    /// @{
    apl::AudioPlayerPtr createPlayer(apl::AudioPlayerCallback playerCallback,
                                     apl::SpeechMarkCallback speechMarkCallback) override;
    /// @}

private:
    std::weak_ptr<AplCoreConnectionManager> m_aplCoreConnectionManager;
    AplConfigurationPtr m_aplConfiguration;
    std::map<std::string, AplCoreAudioPlayerPtr> m_Players;
};

using AplCoreAudioPlayerFactoryPtr = std::shared_ptr<AplCoreAudioPlayerFactory>;

}  // namespace APLClient

#endif  // APLCLIENT_APL_APLCOREAUDIOPLAYERFACTORY_H
