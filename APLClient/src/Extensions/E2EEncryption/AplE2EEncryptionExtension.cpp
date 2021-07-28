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

#include "APLClient/Extensions/E2EEncryption/AplE2EEncryptionExtension.h"

namespace APLClient {
    namespace Extensions {
        namespace E2EEncryption {

            /// String to identify log entries originating from this file.
            static const std::string TAG("AplE2EEncryptionExtension");

            const std::string AplE2EEncryptionExtension::ON_ENCRYPT_SUCCESS = "OnEncryptSuccess";
            const std::string AplE2EEncryptionExtension::ON_BASE64_ENCODE_SUCCESS = "OnBase64EncodeSuccess";
            const std::string AplE2EEncryptionExtension::ON_ENCRYPT_FAILURE = "OnEncryptFailure";
            const std::string AplE2EEncryptionExtension::BASE64_ENCODE = "Base64EncodeValue";
            const std::string AplE2EEncryptionExtension::BASE64_ENCRYPT = "Base64EncryptValue";

            AplE2EEncryptionExtension::AplE2EEncryptionExtension() {}

            std::string AplE2EEncryptionExtension::getUri() {
                return URI;
            }

            apl::Object AplE2EEncryptionExtension::getEnvironment() {
                auto env = std::make_shared<apl::ObjectMap>();

                env->emplace("version", "1.0");

                std::vector<apl::Object> supportedAlgorithms;
                supportedAlgorithms.emplace_back("AES/GCM/NoPadding");
                supportedAlgorithms.emplace_back("RSA/ECB/OAEPWithSHA1AndMGF1Padding");
                supportedAlgorithms.emplace_back("RSA/ECB/OAEPWithSHA256AndMGF1Padding");
                env->emplace("supportedAlgorithms", std::move(supportedAlgorithms));

                return apl::Object(env);
            }

            std::list<apl::ExtensionCommandDefinition> AplE2EEncryptionExtension::getCommandDefinitions() {
                std::list<apl::ExtensionCommandDefinition> extCmdDefs(
                        {
                                apl::ExtensionCommandDefinition(URI, BASE64_ENCODE)
                                        .property("token", "", false)
                                        .property("value", "string", true)
                                        .allowFastMode(true),
                                apl::ExtensionCommandDefinition(URI, BASE64_ENCRYPT)
                                        .property("token", "", false)
                                        .property("value", "string", true)
                                        .property("key", "", false)
                                        .property("algorithm", "string", true)
                                        .property("aad", "", false)
                                        .property("base64Encoded", false, false)
                                        .allowFastMode(true)
                        }
                );
                return extCmdDefs;
            }

            std::list<apl::ExtensionEventHandler> AplE2EEncryptionExtension::getEventHandlers() {
                std::list<apl::ExtensionEventHandler> extensionEventHandlers(
                        {
                                apl::ExtensionEventHandler(URI, ON_BASE64_ENCODE_SUCCESS),
                                apl::ExtensionEventHandler(URI, ON_ENCRYPT_SUCCESS),
                                apl::ExtensionEventHandler(URI, ON_ENCRYPT_FAILURE)
                        }
                );
                return extensionEventHandlers;
            }

            std::unordered_map<std::string, apl::LiveObjectPtr> AplE2EEncryptionExtension::getLiveDataObjects() {
                return std::unordered_map<std::string, apl::LiveObjectPtr>();
            }

            void AplE2EEncryptionExtension::applySettings(const apl::Object &settings) {}

            void AplE2EEncryptionExtension::onExtensionEvent(
                    const std::string &uri,
                    const std::string &name,
                    const apl::Object &source,
                    const apl::Object &params,
                    unsigned int event,
                    std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback) {

                m_eventHandler->sendExtensionEvent(uri, name, source, params, event, resultCallback);
            }
        }
    }
}
