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

#ifndef APLCLIENT_EXTENSIONS_E2EENCRYPTION_H
#define APLCLIENT_EXTENSIONS_E2EENCRYPTION_H

#include "APLClient/Extensions/AplCoreExtensionInterface.h"

namespace APLClient {
namespace Extensions {
namespace E2EEncryption {

static const std::string URI = "aplext:e2eencryption:10";

/**
 * The End-to-end encryption extension allows APL developers the ability to encrypt data client side.
 */
class AplE2EEncryptionExtension :
    public AplCoreExtensionInterface,
    public std::enable_shared_from_this<AplE2EEncryptionExtension> {

public:
    AplE2EEncryptionExtension();

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
            const std::string &uri,
            const std::string &name,
            const apl::Object &source,
            const apl::Object &params,
            unsigned int event,
            std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback = nullptr) override;
    /// @}
private:
    static const std::string ON_ENCRYPT_SUCCESS;
    static const std::string ON_BASE64_ENCODE_SUCCESS;
    static const std::string ON_ENCRYPT_FAILURE;
    static const std::string BASE64_ENCODE;
    static const std::string BASE64_ENCRYPT;
};

using AplE2EEncryptionExtensionionPtr = std::shared_ptr<AplE2EEncryptionExtension>;

}  // namespace E2EEncryption
}  // namespace Extensions
}  // namespace APLClient

#endif //APLCLIENT_EXTENSIONS_E2EENCRYPTION_H
