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

#include "APLClient/Extensions/AttentionSystem/AplAttentionSystemExtension.h"

namespace APLClient {
    namespace Extensions {
        namespace AttentionSystem {

            /// String to identify log entries originating from this file.
            static const std::string TAG("AplAttentionSystemExtension");

            static const std::map<AttentionState, std::string> attentionStateMapping = {
                {AttentionState::IDLE, "IDLE"},
                {AttentionState::LISTENING, "LISTENING"},
                {AttentionState::THINKING, "THINKING"},
                {AttentionState::SPEAKING, "SPEAKING"}
            };

            static const std::string SETTING_ATTENTION_SYSTEM_STATE_NAME = "attentionSystemStateName";
            static const std::string PROPERTY_ATTENTION_STATE = "attentionState";
            static const std::string EVENTHANDLER_ON_ATTENTION_STATE_CHANGED = "OnAttentionStateChanged";

            AplAttentionSystemExtension::AplAttentionSystemExtension() {
                m_attentionSystemStateName = "";
                m_attentionSystemState = apl::LiveMap::create();
                m_attentionSystemState->set(PROPERTY_ATTENTION_STATE, "IDLE");
            }

            std::string AplAttentionSystemExtension::getUri() {
                return URI;
            }

            apl::Object AplAttentionSystemExtension::getEnvironment() {
                auto env = std::make_shared<apl::ObjectMap>();

                env->emplace("version", "1.0");

                return apl::Object(env);
            }

            std::list<apl::ExtensionCommandDefinition> AplAttentionSystemExtension::getCommandDefinitions() {
                return std::list<apl::ExtensionCommandDefinition>();
            }

            std::list<apl::ExtensionEventHandler> AplAttentionSystemExtension::getEventHandlers() {
                std::list<apl::ExtensionEventHandler> extensionEventHandlers(
                        {
                            apl::ExtensionEventHandler(URI, EVENTHANDLER_ON_ATTENTION_STATE_CHANGED)
                        }
                );
                return extensionEventHandlers;
            }

            std::unordered_map<std::string, apl::LiveObjectPtr> AplAttentionSystemExtension::getLiveDataObjects() {
                auto liveObjects = std::unordered_map<std::string, apl::LiveObjectPtr>();
                if (!m_attentionSystemStateName.empty()) {
                    liveObjects.emplace(m_attentionSystemStateName, m_attentionSystemState);
                }
                return liveObjects;
            }

            void AplAttentionSystemExtension::applySettings(const apl::Object &settings) {
                // Reset to defaults
                m_attentionSystemStateName = "";
                /// Apply @c apl::Content defined settings
                logMessage(apl::LogLevel::kInfo, TAG, __func__, settings.toDebugString());
                if (settings.isMap()) {
                    if (settings.has(SETTING_ATTENTION_SYSTEM_STATE_NAME)) {
                        m_attentionSystemStateName = settings.get(SETTING_ATTENTION_SYSTEM_STATE_NAME).getString();
                    }
                }
            }

            void AplAttentionSystemExtension::onExtensionEvent(
                const std::string& uri,
                const std::string& name,
                const apl::Object& source,
                const apl::Object& params,
                unsigned int event,
                std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback) {
            }

            void AplAttentionSystemExtension::updateAttentionSystemState(const AttentionState& state) {
                m_attentionSystemState->set(PROPERTY_ATTENTION_STATE, attentionStateMapping.at(state));

                if (!m_eventHandler) {
                    logMessage(apl::LogLevel::kWarn, TAG, __func__, "No Event Handler");
                    return;
                }

                auto attentionState = apl::ObjectMap({{
                    PROPERTY_ATTENTION_STATE,
                    attentionStateMapping.at(state)
                }});

                m_eventHandler->invokeExtensionEventHandler(
                    URI,
                    EVENTHANDLER_ON_ATTENTION_STATE_CHANGED,
                    attentionState,
                    true
                );
            }
        }
    }
}
