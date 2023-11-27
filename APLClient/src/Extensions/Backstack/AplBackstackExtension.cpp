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
#include "APLClient/Extensions/Backstack/AplBackstackExtension.h"

namespace APLClient {
namespace Extensions {
namespace Backstack {

/// String to identify log entries originating from this file.
static const std::string TAG("AplBackstackExtension");

static const std::string ENV_VERSION = "Backstack-1.0";
static const std::string ENVIRONMENT_RESPONSIBLE_FOR_BACK_BUTTON = "responsibleForBackButton";
static const std::string ENVIRONMENT_BACKSTACK = "backstack";
static const std::string SETTING_PROPERTY_BACKSTACK_ID = "backstackId";
static const std::string SETTING_PROPERTY_BACKSTACK_ARRAY_NAME = "backstackArrayName";
static const std::string COMMAND_GO_BACK_NAME = "GoBack";
static const std::string COMMAND_CLEAR_NAME = "Clear";
static const std::string PROPERTY_BACK_TYPE = "backType";
static const std::string PROPERTY_BACK_VALUE = "backValue";
static const std::string PROPERTY_BACK_TYPE_COUNT = "count";
static const std::string DATA_TYPE_GO_BACK_COMMAND = "GoBackCommandType";
static const std::string DATA_TYPE_ID = "BackstackId";

AplBackstackExtension::AplBackstackExtension(std::shared_ptr<AplBackstackExtensionObserverInterface> observer) :
        alexaext::ExtensionBase(URI),
        m_observer{observer}  {
    m_backstackArrayName = "";
    m_responsibleForBackButton = false;
}

void AplBackstackExtension::setResponsibleForBackButton(bool isResponsibleForBackButton) {
    m_responsibleForBackButton = isResponsibleForBackButton;
}

bool AplBackstackExtension::shouldCacheActiveDocument() {
    return !m_activeDocumentId.empty();
}

void AplBackstackExtension::addDocumentStateToBackstack(const AplDocumentStatePtr& documentState) {
    documentState->id = m_activeDocumentId;
    m_backstack.addDocumentState(documentState);
    clearActiveDocumentId();
}

void AplBackstackExtension::clearActiveDocumentId() {
    m_activeDocumentId = "";
}

void AplBackstackExtension::reset() {
    clearActiveDocumentId();
    m_backstack.clear();
}

bool AplBackstackExtension::handleBack() {
    if (!m_responsibleForBackButton) {
        return goBackCount(1);
    }
    return false;
}

std::string AplBackstackExtension::getUri() {
    return URI;
}

apl::Object AplBackstackExtension::getEnvironment() {
    auto env = std::make_shared<apl::ObjectMap>();
    env->emplace(ENVIRONMENT_RESPONSIBLE_FOR_BACK_BUTTON, m_responsibleForBackButton);
    env->emplace(ENVIRONMENT_BACKSTACK, m_backstack.getBackstackIdsArray());
    return apl::Object(env);
}

std::list<apl::ExtensionCommandDefinition> AplBackstackExtension::getCommandDefinitions() {
    std::list<apl::ExtensionCommandDefinition> extCmdDefs(
        {apl::ExtensionCommandDefinition(URI, COMMAND_GO_BACK_NAME)
             .allowFastMode(true)
             .property(PROPERTY_BACK_TYPE, PROPERTY_BACK_TYPE_COUNT, false)
             .property(PROPERTY_BACK_VALUE, 1, false),
         apl::ExtensionCommandDefinition(URI, COMMAND_CLEAR_NAME).allowFastMode(true)});
    return extCmdDefs;
}

std::list<apl::ExtensionEventHandler> AplBackstackExtension::getEventHandlers() {
    std::list<apl::ExtensionEventHandler> extensionEventHandlers({});
    return extensionEventHandlers;
}

std::unordered_map<std::string, apl::LiveObjectPtr> AplBackstackExtension::getLiveDataObjects() {
    auto liveObjects = std::unordered_map<std::string, apl::LiveObjectPtr>();
    if (!m_backstackArrayName.empty()) {
        liveObjects.emplace(m_backstackArrayName, m_backstack.getBackstackIds());
    }
    return liveObjects;
}

void AplBackstackExtension::applySettings(const apl::Object& settings) {
    // Reset to defaults
    clearActiveDocumentId();
    m_backstackArrayName = "";
    logMessage(LOGLEVEL_DEBUG, TAG, "backstack_settings", settings.toDebugString());
    /// Apply @c apl::Content defined settings
    if (settings.isMap()) {
        if (settings.has(SETTING_PROPERTY_BACKSTACK_ID) && settings.get(SETTING_PROPERTY_BACKSTACK_ID).isString()) {
            m_activeDocumentId = settings.get(SETTING_PROPERTY_BACKSTACK_ID).getString();
        }
        if (settings.has(SETTING_PROPERTY_BACKSTACK_ARRAY_NAME) && settings.get(SETTING_PROPERTY_BACKSTACK_ARRAY_NAME).isString()) {
            m_backstackArrayName = settings.get(SETTING_PROPERTY_BACKSTACK_ARRAY_NAME).getString();
        }
    }
}

bool AplBackstackExtension::restoreDocumentState(const AplDocumentStatePtr& documentState) {
    if (documentState) {
        clearActiveDocumentId();
        m_observer->onRestoreDocumentState(documentState);
        return true;
    }
    return false;
}

bool AplBackstackExtension::goBackId(const std::string& id) {
    return restoreDocumentState(m_backstack.popDocuments(id));
}

bool AplBackstackExtension::goBackIndex(unsigned int index) {
    return restoreDocumentState(m_backstack.popDocumentsAtIndex(index));
}

bool AplBackstackExtension::goBackCount(unsigned int count) {
    return restoreDocumentState(m_backstack.popDocuments(count));
}

bool AplBackstackExtension::handleGoBack(const apl::Object& params) {
    if (confirmEventParams(TAG, {PROPERTY_BACK_TYPE, PROPERTY_BACK_VALUE}, params)) {
        auto backType = backTypeFromString(params.get(PROPERTY_BACK_TYPE).getString());
        auto backValue = params.get(PROPERTY_BACK_VALUE);
        switch (backType) {
            case AplBackType::COUNT:
                if (backValue.isNumber()) {
                    return goBackCount(static_cast<unsigned>(backValue.getDouble()));
                }
            case AplBackType::INDEX:
                if (backValue.isNumber()) {
                    return goBackIndex(static_cast<unsigned>(backValue.getDouble()));
                }
            case AplBackType::ID:
                if (backValue.isString()) {
                    return goBackId(backValue.getString());
                }
        }
    }
    return false;
}

void AplBackstackExtension::onExtensionEvent(
    const std::string& uri,
    const std::string& name,
    const apl::Object& source,
    const apl::Object& params,
    unsigned int event,
    std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback) {
    auto eventDebugString = getEventDebugString(uri, name, params);
    logMessage(LOGLEVEL_DEBUG, TAG, __func__, eventDebugString);

    bool succeeded = true;

    if (m_observer) {
        if (COMMAND_GO_BACK_NAME == name) {
            succeeded = handleGoBack(params);
        } else if (COMMAND_CLEAR_NAME == name) {
            m_backstack.clear();
        } else {
            logMessage(apl::LogLevel::kError, TAG, __func__, "Invalid Command: " + eventDebugString);
            succeeded = false;
        }
    } else {
        logMessage(apl::LogLevel::kError, TAG, __func__, "No Event Observer: " + eventDebugString);
        succeeded = false;
    }

    if (resultCallback) {
        resultCallback->onExtensionEventResult(event, succeeded);
    }
}

/// <alexaext>

rapidjson::Document
AplBackstackExtension::createRegistration(const alexaext::ActivityDescriptor &activity, const rapidjson::Value &registrationRequest) {
    logMessage(LOGLEVEL_DEBUG, TAG, __func__,  "Received registration: " + alexaext::AsPrettyString(registrationRequest));

    const auto &supportedURIs = getURIs();
    const auto& uri = activity.getURI();
    if (supportedURIs.find(uri) == supportedURIs.end()) {
        return alexaext::RegistrationFailure::forUnknownURI(uri);
    }

    applySettings(alexaext::RegistrationRequest::SETTINGS().Get(registrationRequest));

    rapidjson::Document response = alexaext::RegistrationSuccess(alexaext::DEFAULT_SCHEMA_VERSION)
            .environment([&](alexaext::Environment &env) {
                env.version(ENV_VERSION)
                   .property(ENVIRONMENT_RESPONSIBLE_FOR_BACK_BUTTON, m_responsibleForBackButton)
                   .property(ENVIRONMENT_BACKSTACK, m_backstack.getBackstackIdsArrayJson());
            })
            .uri(URI)
            .token("<AUTO_TOKEN>")
            .schema(alexaext::DEFAULT_SCHEMA_VERSION, [&](alexaext::ExtensionSchema &schema) {
                schema.uri(URI);

                schema.dataType(DATA_TYPE_GO_BACK_COMMAND, [](alexaext::TypeSchema &dataType) {
                    dataType.property(PROPERTY_BACK_TYPE, [](alexaext::TypePropertySchema &property) {
                        property
                                .type("string")
                                .required(false)
                                .defaultValue("count");
                    });
                    dataType.property(PROPERTY_BACK_VALUE, [](alexaext::TypePropertySchema &property) {
                        property
                                .type("object") // string or integer, so use object
                                .required(false)
                                .defaultValue(1);
                    });
                });

                schema.dataType(DATA_TYPE_ID);

                schema.command(COMMAND_CLEAR_NAME, [](alexaext::CommandSchema &command) {
                    command.allowFastMode(true);
                });

                schema.command(COMMAND_GO_BACK_NAME, [](alexaext::CommandSchema &command) {
                    command.allowFastMode(true)
                           .dataType(DATA_TYPE_GO_BACK_COMMAND);
                });

                if (!m_backstackArrayName.empty()) {
                    schema.liveDataArray(m_backstackArrayName, [](alexaext::LiveDataSchema &liveData) {
                        liveData.dataType(DATA_TYPE_ID);
                    });
                }
            });

    logMessage(LOGLEVEL_DEBUG, TAG, __func__, "Sending registration response: " + alexaext::AsPrettyString(response));

    return response;
}

void AplBackstackExtension::onSessionStarted(const alexaext::SessionDescriptor& session) {
    logMessage(LOGLEVEL_DEBUG, TAG, __func__, "Session Started");
}

void AplBackstackExtension::onSessionEnded(const alexaext::SessionDescriptor& session) {
    logMessage(LOGLEVEL_DEBUG, TAG, __func__, "Session Ended");
    reset();
}

void AplBackstackExtension::onActivityRegistered(const alexaext::ActivityDescriptor &activity) {
    logMessage(LOGLEVEL_DEBUG, TAG, __func__, "Activity registered: " + activity.getId());
    updateLiveData(activity);
}

void AplBackstackExtension::onActivityUnregistered(const alexaext::ActivityDescriptor& activity) {
    logMessage(LOGLEVEL_DEBUG, TAG, __func__, "Activity unregistered: " + activity.getId());
}

bool AplBackstackExtension::invokeCommand(const alexaext::ActivityDescriptor &activity, const rapidjson::Value &command) {
    logMessage(apl::LogLevel::kInfo, TAG, __func__, "Command invoked: " + alexaext::AsPrettyString(command));

    const std::string &name = alexaext::GetWithDefault(alexaext::Command::NAME(), command, "");

    if (m_observer) {
        if (COMMAND_GO_BACK_NAME == name) {
            const rapidjson::Value* params = alexaext::Command::PAYLOAD().Get(command);
            if (!params || !params->HasMember(PROPERTY_BACK_TYPE) || !params->HasMember(PROPERTY_BACK_VALUE))
                return false;

            auto backType = backTypeFromString(alexaext::GetWithDefault(PROPERTY_BACK_TYPE, params, ""));
            switch (backType) {
                case AplBackType::COUNT: {
                    auto backValue = alexaext::GetWithDefault<unsigned int>(PROPERTY_BACK_VALUE, params, 1);
                    return goBackCount(backValue);
                }
                case AplBackType::INDEX: {
                    auto backValue = alexaext::GetWithDefault<unsigned int>(PROPERTY_BACK_VALUE, params, 1);
                    return goBackIndex(backValue);
                }
                case AplBackType::ID: {
                    auto backValue = alexaext::GetWithDefault(PROPERTY_BACK_VALUE, params, "");
                    return goBackId(backValue);
                }
            }
        } else if (COMMAND_CLEAR_NAME == name) {
            m_backstack.clear();
            return true;
        } else {
            logMessage(apl::LogLevel::kError, TAG, __func__, "Invalid Command: ");
            return false;
        }
    } else {
        logMessage(apl::LogLevel::kError, TAG, __func__, "No Event Observer: ");
        return false;
    }

    return true;
}

void AplBackstackExtension::applySettings(const rapidjson::Value *settings) {
    m_backstackArrayName = alexaext::GetWithDefault(SETTING_PROPERTY_BACKSTACK_ARRAY_NAME, settings, "");
    m_activeDocumentId = alexaext::GetWithDefault(SETTING_PROPERTY_BACKSTACK_ID, settings, "");    
}

void AplBackstackExtension::updateLiveData(const alexaext::ActivityDescriptor &activity) {
    if (m_backstackArrayName.empty()) {
        return;
    }

    rapidjson::Document update = alexaext::LiveDataUpdate(alexaext::DEFAULT_SCHEMA_VERSION)
        .objectName(m_backstackArrayName)
        .uri(getUri())
        .liveDataArrayUpdate([&](alexaext::LiveDataArrayOperation& op) {
            op.type("Clear");
        });

    logMessage(LOGLEVEL_DEBUG, TAG, __func__, "Publishing 'Clear' update: " + alexaext::AsPrettyString(update));

    invokeLiveDataUpdate(activity, update);

    int index = 0;
    for (auto id : m_backstack.getBackstackIdsArray()) {
        rapidjson::Document update = alexaext::LiveDataUpdate(alexaext::DEFAULT_SCHEMA_VERSION)
            .objectName(m_backstackArrayName)
            .uri(getUri())
            .liveDataArrayUpdate([&](alexaext::LiveDataArrayOperation& op) {
                op.type("Insert")
                    .index(index)
                    .item(id.asString());
            });

        logMessage(LOGLEVEL_DEBUG, TAG, __func__, "Publishing 'insert' update: " + alexaext::AsPrettyString(update));
        invokeLiveDataUpdate(activity, update);

        index++;
    }
}

/// </alexaext>

}  // namespace Backstack
}  // namespace Extensions
}  // namespace APLClient
