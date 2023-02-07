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

#include "APLClient/Telemetry/AplMetricsSinkInterface.h"
#include "APLClient/Telemetry/NullAplMetricsRecorder.h"

#include "APLClientSandbox/Logger.h"
#include "APLClientSandbox/Message.h"

#include "APLClientSandbox/AplClientBridge.h"
#include "APLClientSandbox/AplMetricsStreamSink.h"

#include <chrono>

using namespace APLClient::Extensions;
using namespace APLClient::Telemetry;

static const std::chrono::milliseconds RESOURCE_DOWNLOAD_TIMEOUT{3000};

std::shared_ptr<AplClientBridge> AplClientBridge::create() {
    std::shared_ptr<AplClientBridge> client(new AplClientBridge());
    client->m_client = std::make_shared<APLClient::AplClientBinding>(client);
    std::shared_ptr<AplMetricsStreamSink> sink(new AplMetricsStreamSink(client));
    client->m_client->onTelemetrySinkUpdated(sink);
    client->m_aplClientRenderer = client->m_client->createRenderer("");
    client->loadExtensions();
    Logger::info("AplClientBridge::create", "MaxSupportedAPLVersion:", client->m_client->getAPLVersionReported());
    return client;
}

AplClientBridge::AplClientBridge() {
}

void AplClientBridge::loadExtensions() {
    m_backstackExtension = std::make_shared<Backstack::AplBackstackExtension>(shared_from_this());
    m_audioPlayerExtension = std::make_shared<AudioPlayer::AplAudioPlayerExtension>(shared_from_this());
    m_audioPlayerAlarmsExtension = std::make_shared<AudioPlayer::AplAudioPlayerAlarmsExtension>(shared_from_this());

    m_attentionSystemExtension = std::make_shared<AttentionSystem::AplAttentionSystemExtension>();

    addExtensions({
        m_backstackExtension,
        m_audioPlayerExtension,
        m_audioPlayerAlarmsExtension,
        m_attentionSystemExtension
    });
}

void AplClientBridge::addExtensions(std::unordered_set<std::shared_ptr<AplCoreExtensionInterface>> extensions) {
    m_executor.submit([this, extensions] { m_aplClientRenderer->addExtensions(extensions); });
}

void AplClientBridge::updateTick() {
        m_executor.submit([this]() {
        m_aplClientRenderer->onUpdateTick();
        // update audioPlayer
        if (audioPlayerPlaying && m_audioPlayerExtension) {
            int offset = getCurrentTime().count() - audioPlayerStartTime;
            m_audioPlayerExtension->updatePlaybackProgress(offset);
        }
        if (auto manager = m_manager.lock()) {
            manager->onUpdateComplete();
        } else {
            Logger::error("AplClientBridge::sendMessage", "Manager not set");
        }
    });
}

void AplClientBridge::renderDocument(
    const std::string& document,
    const std::string& data,
    const std::string& supportedViewports) {
    m_executor.submit([this, document, data, supportedViewports]() {
        // When rendering a new document, add the current active document state to backstack (if it should be cached)
        if (m_backstackExtension && m_backstackExtension->shouldCacheActiveDocument()) {
            if (auto documentState = m_aplClientRenderer->getActiveDocumentState()) {
                m_backstackExtension->addDocumentStateToBackstack(documentState);
            }
        }

        // When rendering new document, setup audioPlayerExtension session
        if (m_audioPlayerExtension) {
            m_audioPlayerExtension->setActivePresentationSession("sandbox", "sandboxTest");
            audioPlayerStartTime = getCurrentTime().count();
            audioPlayerOffset = 0;
            audioPlayerPlaying = false;
        }

        m_aplClientRenderer->onRenderDirectiveReceived(std::chrono::steady_clock::now());
        m_aplClientRenderer->renderDocument(document, data, supportedViewports, "");
    });
}

void AplClientBridge::clearDocument() {
    m_executor.submit([this]() {
        m_aplClientRenderer->clearDocument();
        if (m_backstackExtension) {
            m_backstackExtension->reset();
        }
    });
}

void AplClientBridge::executeCommands(const std::string& jsonPayload) {
    m_executor.submit([this, jsonPayload]() { m_aplClientRenderer->executeCommands(jsonPayload, ""); });
}

void AplClientBridge::interruptCommandSequence() {
    m_executor.submit([this]() { m_aplClientRenderer->interruptCommandSequence(); });
}

void AplClientBridge::onMessage(const std::string& message) {
    if (m_aplClientRenderer->shouldHandleMessage(message)) {
        m_executor.submit([this, message]() { m_aplClientRenderer->handleMessage(message); });
    }
}

bool AplClientBridge::handleBack() {
    if (m_backstackExtension) {
        return m_backstackExtension->handleBack();
    }
    return false;
}

void AplClientBridge::sendMessage(const std::string& token, const std::string& payload) {
    ViewhostMessage message(payload);
    if (auto manager = m_manager.lock()) {
        manager->sendMessage(message);
    } else {
        Logger::error("AplClientBridge::sendMessage", "Manager not set");
    }
}

void AplClientBridge::resetViewhost(const std::string& token) {
    Logger::debug("AplClientBridge::resetViewhost");
    ResetMessage message;
    if (auto manager = m_manager.lock()) {
        manager->sendMessage(message);
    } else {
        Logger::error("AplClientBridge::resetViewhost", "Manager not set");
    }
}

std::string AplClientBridge::downloadResource(const std::string& source) {
    Logger::debug("AplClientBridge::downloadResource", source);
    ResourceRequestMessage message(source);
    if (auto manager = m_manager.lock()) {
        std::lock_guard<std::mutex> mtx(m_downloadMutex);
        m_resourcePromiseUrl = source;
        m_resourcePromise = std::promise<std::string>();
        manager->sendMessage(message);
        auto future = m_resourcePromise.get_future();
        auto status = future.wait_for(RESOURCE_DOWNLOAD_TIMEOUT);
        if (status != std::future_status::ready) {
            Logger::error("AplClientBridge::downloadResource", "Did not receive reply for resource request");
            return "";
        } else {
            return future.get();
        }
    } else {
        Logger::error("AplClientBridge::downloadResource", "Manager not set");
        return "";
    }
}

std::chrono::milliseconds AplClientBridge::getTimezoneOffset() {
    return std::chrono::milliseconds();
}

void AplClientBridge::onActivityStarted(const std::string& token, const std::string& source) {
    Logger::debug("AplClientBridge::onActivityStarted", source);
}

void AplClientBridge::onActivityEnded(const std::string& token, const std::string& source) {
    Logger::debug("AplClientBridge::onActivityEnded", source);
}

void AplClientBridge::onSendEvent(const std::string& token, const std::string& event) {
    Logger::info("AplClientBridge::onSendEvent", event);
}

void AplClientBridge::onCommandExecutionComplete(const std::string& token, APLClient::AplCommandExecutionEvent event, const std::string& message) {
    Logger::info("AplClientBridge::onCommandExecutionComplete", "event:", APLClient::commandExecutionEventToString(event), ", message:", message);
}

void AplClientBridge::onRenderDocumentComplete(const std::string& token, bool result, const std::string& error) {
    Logger::info("AplClientBridge::onRenderDocumentComplete", "success:", result, ", error:", error);
}

void AplClientBridge::onVisualContextAvailable(
        const std::string& token,
        unsigned int stateRequestToken,
        const std::string& context) {
    Logger::info("AplClientBridge::onVisualContextAvailable", context);
}

void AplClientBridge::onSetDocumentIdleTimeout(
        const std::string& token,
        const std::chrono::milliseconds& timeout) {
    Logger::info("AplClientBridge::onSetDocumentIdleTimeout", "ms: ", timeout.count());
}

void AplClientBridge::onRenderingEvent(const std::string& token, APLClient::AplRenderingEvent event) {
    Logger::debug("AplClientBridge::onRenderingEvent");
    m_aplClientRenderer->onRenderingEvent(event);
}

void AplClientBridge::onFinish(const std::string& token) {
    Logger::info("AplClientBridge::onFinish");
}

void AplClientBridge::onDataSourceFetchRequestEvent(
        const std::string& token,
        const std::string& type,
        const std::string& payload) {
    Logger::info("AplClientBridge::onDataSourceFetchRequestEvent", type, payload);
}

void AplClientBridge::onRuntimeErrorEvent(const std::string& token, const std::string& payload) {
    Logger::warn("AplClientBridge::onRuntimeErrorEvent", payload);
}

void AplClientBridge::onExtensionEvent(
    const std::string& aplToken,
    const std::string& uri,
    const std::string& name,
    const std::string& source,
    const std::string& params,
    unsigned int event,
    std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback) {
    Logger::info("AplClientBridge::onExtensionEvent");

    m_executor.submit([this, uri, name, source, params, event, resultCallback] {
        m_aplClientRenderer->onExtensionEvent(uri, name, source, params, event, resultCallback);
    });
}

void AplClientBridge::logMessage(APLClient::LogLevel level, const std::string& source, const std::string& message) {
    switch (level) {
        case APLClient::LogLevel::CRITICAL:
        case APLClient::LogLevel::ERROR:
            Logger::error("AplClientBridge::logMessage", "source:", source, ", message:", message);
            break;
        case APLClient::LogLevel::WARN:
            Logger::warn("AplClientBridge::logMessage", "source:", source, ", message:", message);
            break;
        case APLClient::LogLevel::INFO:
            Logger::info("AplClientBridge::logMessage", "source:", source, ", message:", message);
            break;
        case APLClient::LogLevel::DBG:
        case APLClient::LogLevel::TRACE:
            Logger::debug("AplClientBridge::logMessage", "source:", source, ", message:", message);
            break;
        default:
            Logger::error("AplClientBridge::logMessage", "UNKNOWN LOGLEVEL", "source:", source, ", message:", message);
            break;
    }
}

void AplClientBridge::setGUIManager(std::shared_ptr<GUIManager> manager) {
    m_manager = std::move(manager);
}

void AplClientBridge::provideResource(const std::string& url, const std::string& payload) {
    if (url != m_resourcePromiseUrl) {
        Logger::warn("AplClientBridge::provideResource", "Received resource for different url than expected");
    } else {
        m_resourcePromise.set_value(payload);
    }
}

int AplClientBridge::getMaxNumberOfConcurrentDownloads() {
    return 5;
}

void AplClientBridge::onRestoreDocumentState(std::shared_ptr<APLClient::AplDocumentState> documentState) {
    m_executor.submit([this, documentState]() { m_aplClientRenderer->restoreDocumentState(documentState); });
}

void AplClientBridge::onAudioPlayerPlay() {
    Logger::info("AplClientBridge::onAudioPlayerPlay", "Play");
    if (!audioPlayerPlaying) {
        audioPlayerStartTime = getCurrentTime().count() - audioPlayerOffset;
        audioPlayerPlaying = true;
    }
    if (m_audioPlayerExtension) {
        m_audioPlayerExtension->updatePlayerActivity("PLAYING", audioPlayerOffset);
    }
}

void AplClientBridge::onAudioPlayerPause() {
    Logger::info("AplClientBridge::onAudioPlayerPause", "Pause");
    if (audioPlayerPlaying) {
        audioPlayerOffset = getCurrentTime().count() - audioPlayerStartTime;
        audioPlayerPlaying = false;
        if (m_audioPlayerExtension) {
            m_audioPlayerExtension->updatePlayerActivity("PAUSED", audioPlayerOffset);
        }
    }
}

void AplClientBridge::onAudioPlayerNext() {
    Logger::info("AplClientBridge::onAudioPlayerNext", "Next");
}

void AplClientBridge::onAudioPlayerPrevious() {
    Logger::info("AplClientBridge::onAudioPlayerPrevious", "Previous");
}

void AplClientBridge::onAudioPlayerSeekToPosition(int offsetInMilliseconds) {
    Logger::info("AplClientBridge::onAudioPlayerSeekToPosition", "AudioPlayerSeekToPosition", offsetInMilliseconds);
    audioPlayerOffset = offsetInMilliseconds;
    audioPlayerStartTime = getCurrentTime().count() - audioPlayerOffset;
    if (m_audioPlayerExtension) {
        m_audioPlayerExtension->updatePlaybackProgress(audioPlayerOffset);
    }
}

void AplClientBridge::onAudioPlayerToggle(const std::string &name, bool checked) {
    Logger::info("AplClientBridge::onAudioPlayerToggle", "onAudioPlayerToggle", name, checked);
}

void AplClientBridge::onAudioPlayerLyricDataFlushed(const std::string &token, long durationInMilliseconds,
                                                    const std::string &lyricData) {
    Logger::info("AplClientBridge::onAudioPlayerLyricDataFlushed", "FlushLyricData", token, durationInMilliseconds, lyricData);
}

void AplClientBridge::onAudioPlayerSkipForward() {
    Logger::info("AplClientBridge::onAudioPlayerSkipForward", "SkipForward");
}

void AplClientBridge::onAudioPlayerSkipBackward() {
    Logger::info("AplClientBridge::onAudioPlayerSkipBackward", "SkipBackward");
}

void AplClientBridge::onAudioPlayerAlarmDismiss() {
    Logger::info("AplClientBridge::onAudioPlayerAlarmDismiss", "AlarmDismiss");
}

void AplClientBridge::onAudioPlayerAlarmSnooze() {
    Logger::info("AplClientBridge::onAudioPlayerAlarmSnooze", "AlarmSnooze");
}

const std::map<std::string, APLClient::Extensions::AttentionSystem::AttentionState> attentionStateMapping = {
    {"IDLE", APLClient::Extensions::AttentionSystem::AttentionState::IDLE},
    {"LISTENING", APLClient::Extensions::AttentionSystem::AttentionState::LISTENING},
    {"THINKING", APLClient::Extensions::AttentionSystem::AttentionState::THINKING},
    {"SPEAKING", APLClient::Extensions::AttentionSystem::AttentionState::SPEAKING}
};

void AplClientBridge::updateAttentionSystemState(const std::string& state) {
    Logger::info("AplClientBridge::updateAttentionSystemState", "updateAttentionSystemState", state);
    if (m_attentionSystemExtension) {
        if (attentionStateMapping.find(state) == attentionStateMapping.end()) {
            Logger::info("AplClientBridge::updateAttentionSystemState", "Invalid Attention System State: ", state);
            return;
        } else {
            m_attentionSystemExtension->updateAttentionSystemState(attentionStateMapping.at(state));
        }
    }
}

void AplClientBridge::processDataSourceUpdate(const std::string& updateIndexListData, const std::string& dynamicDataSourceType) {
    Logger::info("AplClientBridge::processDataSourceUpdate", updateIndexListData, dynamicDataSourceType);
    m_executor.submit([this, updateIndexListData, dynamicDataSourceType]() { m_aplClientRenderer->dataSourceUpdate(dynamicDataSourceType, updateIndexListData, ""); });
}