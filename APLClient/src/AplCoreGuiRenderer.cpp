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
#include <fstream>

#include <rapidjson/document.h>

#include "APLClient/AplCoreGuiRenderer.h"
#include "APLClient/AplSession.h"

namespace APLClient {

/// Name of the mainTemplate parameter to which avs datasources binds to.
static const std::string DEFAULT_PARAM_BINDING = "payload";
/// Default string to attach to mainTemplate parameters.
static const std::string DEFAULT_PARAM_VALUE = "{}";

AplCoreGuiRenderer::AplCoreGuiRenderer(AplConfigurationPtr config, AplCoreConnectionManagerPtr aplCoreConnectionManager)
        : m_aplConfiguration{config},
          m_aplCoreConnectionManager{aplCoreConnectionManager},
          m_isDocumentCleared{false} {

}

void AplCoreGuiRenderer::executeCommands(const std::string& jsonPayload, const std::string& token) {
    m_aplCoreConnectionManager->executeCommands(jsonPayload, token);
}

void AplCoreGuiRenderer::dataSourceUpdate(
    const std::string& sourceType,
    const std::string& jsonPayload,
    const std::string& token) {
    m_aplCoreConnectionManager->dataSourceUpdate(sourceType, jsonPayload, token);
}

void AplCoreGuiRenderer::interruptCommandSequence() {
    m_aplCoreConnectionManager->interruptCommandSequence();
}

void AplCoreGuiRenderer::setViewhostConfig(const AplViewhostConfigPtr& config) {
    m_aplCoreConnectionManager->updateViewhostConfig(config);
}

void AplCoreGuiRenderer::renderDocument(
    const std::string& document,
    const std::string& data,
    const std::string& supportedViewports,
    const std::string& token) {
    m_isDocumentCleared = false;

    auto metricsRecorder = m_aplConfiguration->getMetricsRecorder();
    auto aplOptions = m_aplConfiguration->getAplOptions();
    auto tContentCreate = metricsRecorder->createTimer(
            Telemetry::AplMetricsRecorderInterface::LATEST_DOCUMENT,
            Telemetry::AplRenderingSegment::kContentCreation);
    auto cImports = metricsRecorder->createCounter(
            Telemetry::AplMetricsRecorderInterface::LATEST_DOCUMENT,
            "APL-Web.Content.imports");
    auto cError = metricsRecorder->createCounter(
            Telemetry::AplMetricsRecorderInterface::LATEST_DOCUMENT,
            "APL-Web.Content.error");
    tContentCreate->start();

    const auto aplViewhostConfig = m_aplCoreConnectionManager->getViewhostConfig();
    const bool isLogCommandEnabled = aplViewhostConfig ? aplViewhostConfig->isLogCommandEnabled() : false;
    auto content = apl::Content::create(std::move(document), std::make_shared<AplSession>(isLogCommandEnabled),
                                        m_aplCoreConnectionManager->getMetrics(), m_aplCoreConnectionManager->getRootConfig());

    if (!content) {
        aplOptions->logMessage(LogLevel::ERROR, "renderByAplCoreFailed", "Unable to create content");

        tContentCreate->fail();
        aplOptions->onRenderDocumentComplete(token, false, "Unable to create content");
        return;
    }

    std::map<std::string, apl::JsonData> params;
    apl::JsonData sourcesData(data);
    if (sourcesData.get().IsObject()) {
        for (auto objIt = sourcesData.get().MemberBegin(); objIt != sourcesData.get().MemberEnd(); objIt++) {
            params.emplace(objIt->name.GetString(), objIt->value);
        }
    }

    for (size_t idx = 0; idx < content->getParameterCount(); idx++) {
        auto parameterName = content->getParameterAt(idx);
        if (parameterName == DEFAULT_PARAM_BINDING) {
            content->addData(parameterName, data);
        } else if (params.find(parameterName) != params.end()) {
            content->addData(parameterName, params.at(parameterName).toString());
        } else {
            content->addData(parameterName, DEFAULT_PARAM_VALUE);
        }
    }

    if (!m_aplCoreConnectionManager->loadPackage(content)) {
        aplOptions->onRenderDocumentComplete(token, false, "Unresolved import");
        tContentCreate->fail();
        cError->increment();
        return;
    }

    if (!content->isReady()) {
        aplOptions->logMessage(LogLevel::ERROR, "renderByAplCoreFailed", "Content is not ready");

        aplOptions->onRenderDocumentComplete(token, false, "Content is not ready");
        tContentCreate->fail();
        return;
    }

    tContentCreate->stop();
    metricsRecorder->flush();

    if (!m_isDocumentCleared) {
        /**
         *  Only set the content if we haven't been cleared while building.
         */
        m_aplCoreConnectionManager->setSupportedViewports(supportedViewports);
        m_aplCoreConnectionManager->setContent(content, token);
    }
}

void AplCoreGuiRenderer::clearDocument() {
    m_isDocumentCleared = true;
    m_aplCoreConnectionManager->reset();
}

}  // namespace APLClient
