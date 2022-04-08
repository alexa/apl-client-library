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

#ifndef APLCLIENT_EXTENSIONS_ATTENTIONSYSTEM_H
#define APLCLIENT_EXTENSIONS_ATTENTIONSYSTEM_H

#include "APLClient/Extensions/AplCoreExtensionInterface.h"

namespace APLClient {
namespace Extensions {
namespace AttentionSystem {

enum class AttentionState {
    IDLE,
    LISTENING,
    THINKING,
    SPEAKING
};

static const std::string URI = "aplext:attentionsystem:10";

/**
 * The Attention system extension allows APL developers the ability to react to changes in the attention system state.
 */
class AplAttentionSystemExtension :
    public AplCoreExtensionInterface,
    public std::enable_shared_from_this<AplAttentionSystemExtension> {

public:
    AplAttentionSystemExtension();

    /// @name AplCoreExtensionInterface Functions
    /// @{
    std::string getUri() override;

    apl::Object getEnvironment() override;

    std::list<apl::ExtensionCommandDefinition> getCommandDefinitions() override;

    std::list<apl::ExtensionEventHandler> getEventHandlers() override;

    std::unordered_map<std::string, apl::LiveObjectPtr> getLiveDataObjects() override;

    void applySettings(const apl::Object &settings) override;
    /// @}

    /// @name AplCoreExtensionEventCallbackInterface Functions
    /// @{
    void onExtensionEvent(
        const std::string& uri,
        const std::string& name,
        const apl::Object& source,
        const apl::Object& params,
        unsigned int event,
        std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback = nullptr
    ) override;
    /// @}

    /**
     * Call to invoke the OnAttentionStateChanged ExtensionEventHandler and update the AttentionSystemState apl::LiveMap.
     * It is expected that this is called on every change in the AttentionSystem's attention state.
     *
     * @param state The system's attention state
     */
    void updateAttentionSystemState(const AttentionState& state);
private:
    /// The document settings defined 'name' for the attentionSystemState data object
    std::string m_attentionSystemStateName;

    /// The @c apl::LiveMap for AttentionSystem attentionSystemState data.
    apl::LiveMapPtr m_attentionSystemState;
};

using AplAttentionSystemExtensionionPtr = std::shared_ptr<AplAttentionSystemExtension>;

}  // namespace AttentionSystem
}  // namespace Extensions
}  // namespace APLClient

#endif //APLCLIENT_EXTENSIONS_ATTENTIONSYSTEM_H
