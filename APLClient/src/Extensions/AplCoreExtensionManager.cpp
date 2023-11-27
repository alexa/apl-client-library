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

#include <string>
#include "APLClient/Extensions/AplCoreExtensionManager.h"

namespace APLClient {
namespace Extensions {

/// String to identify log entries originating from this file.
static const std::string TAG("AplCoreExtensionManager");

// ---------------- 
// AplCoreExtensionInterface
// a.k.a. "legacy" extensions 
// ---------------- 

std::shared_ptr<AplCoreExtensionInterface> AplCoreExtensionManager::getExtension(const std::string& uri) {
    logMessage(LOGLEVEL_DEBUG, TAG, __func__, uri);
    if (m_Extensions.find(uri) != m_Extensions.end()) {
        return m_Extensions[uri];
    }
    logMessage(LOGLEVEL_DEBUG, TAG, "getExtension", " No registered Extension: " + uri);
    return nullptr;
}

void AplCoreExtensionManager::addExtension(std::shared_ptr<AplCoreExtensionInterface> extension) {
    // Setting the allowed extension type to: Legacy
    if (!m_ExtensionsHaveBeenAdded) {
        m_ExtensionsHaveBeenAdded = true;
        m_UseAlexaExt = false;
    }

    if (m_UseAlexaExt) {
        logMessage(LOGLEVEL_ERROR, TAG, "addExtension", "You can only register one type of extension. At least one AlexaExt has already been registed.");
        return;
    }

    if (!getExtension(extension->getUri())) {
        logMessage(LOGLEVEL_ERROR, TAG, "addExtension", "Legacy extension added: " + extension->getUri());
        m_Extensions.insert({extension->getUri(), extension});
    }
}

void AplCoreExtensionManager::registerRequestedExtension(const std::string& uri, apl::RootConfig& config) {
    if (auto extension = getExtension(uri)) {
        logMessage(LOGLEVEL_DEBUG, TAG, "registerRequestedExtension", uri);
        config.registerExtension(uri);
        config.registerExtensionEnvironment(extension->getUri(), extension->getEnvironment());
        for (auto& command : extension->getCommandDefinitions()) {
            logMessage(LOGLEVEL_DEBUG, TAG, "registerExtensionCommand", command.toDebugString());
            config.registerExtensionCommand(command);
        }
        for (auto& handler : extension->getEventHandlers()) {
            logMessage(LOGLEVEL_DEBUG, TAG, "registerExtensionEventHandler", handler.toDebugString());
            config.registerExtensionEventHandler(handler);
        }
        // Add Extension LiveData Objects to config
        auto extensionLiveObjects = extension->getLiveDataObjects();
        auto it = extensionLiveObjects.begin();
        while (it != extensionLiveObjects.end())
        {
            config.liveData(it->first, it->second);
            it++;
        }
    }
}

void AplCoreExtensionManager::onExtensionEvent(
    const std::string& uri,
    const std::string& name,
    const apl::Object& source,
    const apl::Object& params,
    unsigned int event,
    std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback) {
    logMessage(LOGLEVEL_DEBUG, TAG, "extensionEvent", +"< " + uri + "::" + name + "::" + params.toDebugString() + " >");
    if (auto extension = getExtension(uri)) {
        extension->onExtensionEvent(uri, name, source, params, event, resultCallback);
    } else if (resultCallback) {
        resultCallback->onExtensionEventResult(event, false);
    }
}

// ---------------- 
// AlexaExt
// ---------------- 

void AplCoreExtensionManager::addAlexaExtExtension(const alexaext::ExtensionPtr& extension) {
    if (!m_ExtensionsHaveBeenAdded) {
        // Setting the allowed extension type to: AlexaExt
        m_ExtensionsHaveBeenAdded = true;
        m_UseAlexaExt = true;
    }

    if (!m_UseAlexaExt) {
        logMessage(LOGLEVEL_ERROR, TAG, "addAlexaExtExtension", "You can only register one type of extension. At least one legacy extension has already been registed.");
        return;
    }

    for (const auto& uri : extension->getURIs()) {
        if (!getAlexaExtExtension(uri)) {
            logMessage(LOGLEVEL_DEBUG, TAG, "addAlexaExtExtension", "AlexaExt added: " + uri);
            m_AlexaExtExtensions.insert({uri, extension});
        }
    }
}

alexaext::ExtensionPtr AplCoreExtensionManager::getAlexaExtExtension(const std::string& uri) {
    logMessage(LOGLEVEL_DEBUG, TAG, __func__, uri);

    if (m_AlexaExtExtensions.find(uri) != m_AlexaExtExtensions.end()) {
        return m_AlexaExtExtensions[uri];
    }
    logMessage(LOGLEVEL_DEBUG, TAG, "getAlexaExtExtension", "No registered AlexaExt Extension: " + uri);
    return nullptr;
}

bool AplCoreExtensionManager::useAlexaExt() {
    return m_UseAlexaExt;
}

void AplCoreExtensionManager::setExtensionRegistrar(const alexaext::ExtensionRegistrarPtr& registrar) {
    m_extensionRegistrar = registrar;
};

alexaext::ExtensionRegistrarPtr AplCoreExtensionManager::getExtensionRegistrar() { 
    return m_extensionRegistrar; 
};

void AplCoreExtensionManager::setExtensionExecutor(const AlexaExtExtensionExecutorPtr& executor) {
    m_extensionExecutor = executor;
};

AlexaExtExtensionExecutorPtr AplCoreExtensionManager::getExtensionExecutor() { 
    return m_extensionExecutor; 
};

}  // namespace Extensions
}  // namespace APLClient
