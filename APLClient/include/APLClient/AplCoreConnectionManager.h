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

#ifndef APL_CLIENT_LIBRARY_APL_CORE_CONNECTION_MANAGER_H_
#define APL_CLIENT_LIBRARY_APL_CORE_CONNECTION_MANAGER_H_

#include <string>
#include <unordered_set>
#include <future>
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

#include <alexaext/alexaext.h>

#include "AplConfiguration.h"
#include "AplCoreViewhostMessage.h"
#include "AplCoreMetrics.h"
#include "AplViewhostConfig.h"
#include "Extensions/AplCoreExtensionEventCallbackResultInterface.h"
#include "Extensions/AplCoreExtensionEventHandlerInterface.h"
#include "Extensions/AplCoreExtensionInterface.h"
#include "Extensions/AplCoreExtensionManager.h"
#include "Extensions/AplDocumentState.h"
#include "Extensions/AplCoreExtensionSupportedExtension.h"
#include "Telemetry/AplMetricsRecorderInterface.h"
#include "APLClient/Extensions/AplCoreExtensionExecutor.h"

namespace APLClient {

using namespace APLClient::Extensions;

class AplCoreAudioPlayerFactory;
class AplCoreMediaPlayerFactory;

/**
 * Interacts with the APL Core Engine handling the event loop, updates etc. and passes messages between the core
 * and the viewhost.
 */
class AplCoreConnectionManager
        : public AplCoreExtensionEventHandlerInterface
        , public AplCoreExtensionEventCallbackResultInterface
        , public std::enable_shared_from_this<AplCoreConnectionManager> {
public:
    AplCoreConnectionManager(AplConfigurationPtr config);

    virtual ~AplCoreConnectionManager() = default;

public:
    /// @name AplCoreExtensionEventHandlerInterface Functions
    /// @{
    void invokeExtensionEventHandler(
        const std::string& uri,
        const std::string& name,
        const apl::ObjectMap& data,
        bool fastMode) override;

    void sendExtensionEvent(
        const std::string& uri,
        const std::string& name,
        const apl::Object& source,
        const apl::Object& params,
        unsigned int event,
        std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback) override;
    /// @}

    /// @name AplCoreExtensionEventCallbackResultInterface Functions
    /// @{
    void onExtensionEventResult(unsigned int event, bool succeeded) override;
    /// @}

    /**
     * Sets the APL Content to be rendered by the APL Core
     * @param content
     * @param token APL Presentation token for this content
     */
    virtual void setContent(const apl::ContentPtr content, const std::string& token);

    /**
     * Sets the APL ScalingOptions
     * @param supportedViewports The JSON Payload
     */
    virtual void setSupportedViewports(const std::string& jsonPayload);

    /**
     * Receives messages from the APL view host and identifies if it will require further handling
     * @note This function does not need to be handled on the same execution thread as other function calls
     * @param message The JSON Payload
     * @return true if the message should be passed to @c handleMessage, false if message
     */
    bool shouldHandleMessage(const std::string& message);

    /**
     * Receives messages from the APL view host
     * @param message The JSON Payload
     */
    void handleMessage(const std::string& message);

    /**
     * Executes an APL command
     * @param command The command to execute
     * @param token Directive token to bind result processing
     */
    void executeCommands(const std::string& command, const std::string& token);

    /**
     * Execute DataSource updates.
     * @param sourceType DataSource type.
     * @param jsonPayload The payload of the directive in structured JSON format.
     * @param token Directive token used to bind result processing.
     */
    void dataSourceUpdate(const std::string& sourceType, const std::string& jsonPayload, const std::string& token);

    /**
     * Interrupts the currently executing APL command sequence
     */
    void interruptCommandSequence();

    /**
     * Send a message to the view host
     * @param message The message to send
     * @return The sequence number of this message
     */
    unsigned int send(AplCoreViewhostMessage& message);

    /**
     * Send a message to the view host and block until you get a reply
     * @param message The message to send
     * @return The resultant message or a NULL object if a response was not received.
     */
    rapidjson::Document blockingSend(
        AplCoreViewhostMessage& message,
        const std::chrono::milliseconds& timeout = std::chrono::milliseconds(2000));

    void provideState(unsigned int stateRequestToken);

    AplCoreMetricsPtr aplCoreMetrics() const {
        return m_AplCoreMetrics;
    }

    /**
     * Schedules an update on the root context and runs the update loop - this may result in the viewhost being
     * updated and any events currently pending will be processed. If nothing is currently being displayed calling
     * this method will result in no action being taken. For the best rendering experience it is recommended that this
     * method is scheduled to be called at a rate which matches the refresh rate of the users display.
     */
    void onUpdateTick();

    /**
     * Resets the connection manager to remove the current document
     */
    void reset();

    const std::string getAPLToken();

    /**
     * Adds Extensions to the client
     * @param extensions Set of Shared Pointers to AplCoreExtensionInterfaces
     */
    void addExtensions(std::unordered_set<std::shared_ptr<AplCoreExtensionInterface>> extensions);

    /**
     * Adds Extensions to the client
     * @param extensions Set of Shared Pointers to alexaext::Extension
     */
    void addAlexaExtExtensions(
        const std::unordered_set<alexaext::ExtensionPtr>& extensions,
        const alexaext::ExtensionRegistrarPtr& registrar,
        const AlexaExtExtensionExecutorPtr& executor
    );

    /**
     * Gets the requested extension from the client
     * @param uri Extension Uri
     * @return Shared pointer to @c AplCoreExtensionInterface
     */
    std::shared_ptr<AplCoreExtensionInterface> getExtension(const std::string& uri);

    /**
     * Gets the requested Alexaext extension from the client
     * @param uri Extension Uri
     * @return Shared pointer to @c alexaext::Extension 
     */
    alexaext::ExtensionPtr getAlexaExtExtension(const std::string& uri);

    /**
     * Extension Event Callback function to invoke an extension event registered with @c AplCoreExtensionManager
     * @param uri Extension uri
     * @param name Extension event name
     * @param source Map of the source object that raised the event
     * @param params Map of the user-specified properties
     * @param event Event number
     * @param resultCallback Pointer to result callback interface
     */
    void onExtensionEvent(
        const std::string& uri,
        const std::string& name,
        const std::string& source,
        const std::string& params,
        unsigned int event,
        std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback);

    /**
     * Retrieve the active @c AplDocumentState.
     * @return The active @c AplDocumentState.
     */
    AplDocumentStatePtr getActiveDocumentState();

    /**
     * Restore content from provided @c AplDocumentState
     * @param documentState the @c AplDocumentState to restore.
     */
    void restoreDocumentState(AplDocumentStatePtr documentState);

    // Initialise Extensions provided via the AlexaExt flow (alexaext::Extension)
    bool initAlexaExts(const std::set<std::string>& requestedExtensions);

    // Initialise Extensions provided via the Legacy flow (APLCoreExtensionInterface)
    void initLegacyExts(const std::set<std::string>& requestedExtensions);

    /**
     * Called when a document has finished rendering.
     *
     * @param renderTime the timestamp at which rendering ended
     * @param complexityScore the complexity score received from the viewhost, or 0 if unknown.
     */
     void onDocumentRendered(const std::chrono::steady_clock::time_point &renderTime, uint64_t complexityScore);

    /**
     * Retrieves the current time
     * @return The time
     */
    std::chrono::milliseconds getCurrentTime() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch());
    }

    /**
     * @brief Load the package required by the content
     * virtual for testing
     */
    virtual bool loadPackage(const apl::ContentPtr& content);

    /**
     * @brief Updates the cached viewhost config, will also update the default root config
     * 
     * @param viewhostConfig 
     */
    void updateViewhostConfig(const AplViewhostConfigPtr viewhostConfig);

    const apl::RootConfig& getRootConfig() const {
        return m_RootConfig;
    }

    const apl::Metrics& getMetrics() const {
        return m_Metrics;
    }

private:
    /**
     * Sends viewport scaling information to the client
     */
    void sendViewhostScalingMessage();

    /**
     * Sends document background information to the client
     * @param background
     */
    void sendDocumentBackgroundMessage(const apl::Object& background);

    /**
     * Sends screenLock state to the client
     * @param screenLock
     */
    void sendScreenLockMessage(bool screenLock);

    /**
     * Sends supportsResizing state to the client
     * @param screenLock
     */
    void sendSupportsResizingMessage(bool supportsResizing);

    /**
     * Handles the build message received from the view host, builds the component hierarchy.
     * @param message
     */
    void handleBuild(const rapidjson::Value& message);

    /**
     * Handles the configuration change message from the view host and send to Core.
     * @param message
     */
    void handleConfigurationChange(const rapidjson::Value& message);

    /**
     * Handles the update display state message from the view host and send to Core.
     * @param message
     */
    void handleUpdateDisplayState(const rapidjson::Value& message);

    /**
     * Handle an update message from the view host of the form:
     *
     *     { "id": COMPONENT_ID, "type": EventType(int), "value": Integer }
     *
     * @param update
     */
    void handleUpdate(const rapidjson::Value& update);

    /**
     * Handle an media update message from the view host of the form:
     *
     *     { "id": COMPONENT_ID, "mediaState": apl::MediaState, "fromEvent": boolean }
     *
     * @param update
     */
    void handleMediaUpdate(const rapidjson::Value& update);

    /**
     * Handle an graphic update message from the view host of the form:
     *
     *     { "id": COMPONENT_ID, "avg": json }
     *
     * @param update
     */
    void handleGraphicUpdate(const rapidjson::Value& update);

    /**
     * Handles the ensureLayout message received from the viewhost
     * @param payload
     */
    void handleEnsureLayout(const rapidjson::Value& payload);

    /**
     * Handle the scrollToRectInComponent message received from the viewhost
     * @param payload
     */
    void handleScrollToRectInComponent(const rapidjson::Value& payload);

    /**
     * Handle the handleKeyboard message received from the viewhost
     * @param payload
     */
    void handleHandleKeyboard(const rapidjson::Value& payload);

    /**
     * Handle the setFocus message received from the viewhost
     * @param payload
     */
    void setFocus(const rapidjson::Value& payload);

    /**
     * Handle the mediaLoaded message received from the viewhost
     * @param payload
     */
    void mediaLoaded(const rapidjson::Value& payload);

    /**
     * Handle the mediaLoadFailed message received from the viewhost
     * @param payload
     */
    void mediaLoadFailed(const rapidjson::Value& payload);

    /**
     * Handle the audioPlayerCallback message received from the viewhost
     * @param payload
     */
    void audioPlayerCallback(const rapidjson::Value& payload);

    /**
     * Handle the audioPlayerSpeechMarks message received from the viewhost
     * @param payload
     */
    void audioPlayerSpeechMarks(const rapidjson::Value& payload);

    /**
     * Handle the mediaPlayerUpdateMediaState message received from the viewhost
     * @param payload
     */
    void mediaPlayerUpdateMediaState(const rapidjson::Value& payload);

    /**
     * Handle the mediaPlayerDoCallback message received from the viewhost
     * @param payload
     */
    void mediaPlayerDoCallback(const rapidjson::Value& payload);

    /**
     * Handle the getFocusableAreas message received from the viewhost
     * @param payload
     */
    void getFocusableAreas(const rapidjson::Value& payload);

    /**
     * Handle the getFocused message received from the viewhost
     * @param payload
     */
    void getFocused(const rapidjson::Value& payload);

    /**
     * Handle the extension message received from the viewhost
     * @param payload
     */
    void handleExtensionMessage(const rapidjson::Value& payload);

    /**
     * Handle the updateCursorPosition message received from the viewhost
     * @param payload
     */
    void handleUpdateCursorPosition(const rapidjson::Value& payload);

    /**
     * Handle the handle handlePointerEvent message received from the viewhost
     * @param payload
     */
    void handleHandlePointerEvent(const rapidjson::Value& payload);

    /**
     * Process responses to events with action references.  The payload should be of the form:
     *
     *    { "event": EVENT_NUMBER, "argument": VALUE }
     *
     * @param response
     */
    void handleEventResponse(const rapidjson::Value& response);

    /**
     * Check for screenLock condition and process it accordingly.
     */
    void handleScreenLock();

    /**
     * Handle the isCharacterValid message received from ViewHost.
     * @param payload
     */
    void handleIsCharacterValid(const rapidjson::Value& payload);

    /**
     * Handle the reInflate message received from ViewHost.
     * @param payload
     */
    void handleReInflate(const rapidjson::Value& payload);

    /**
     * Handle the reHierarchy message received from ViewHost.
     * @param payload
     */
    void handleReHierarchy(const rapidjson::Value& payload);

    /**
     * Execute the event.
     * ActionRefs have to be stored while we are waiting for a response.
     * Terminates have to be sent up if the action is cancelled.
     * Resolves/Rejects sent down have to be acted up.
     * @param event requested core event.
     */
    void processEvent(const apl::Event& event);

    /**
     * Adds events with @c ActionRefs to the pending event stack
     * @param token event number.
     * @param event the apl core event.
     * @param isViewhostEvent is this a viewhost handled event.
     */
    bool addPendingEvent(unsigned int token, const apl::Event& event, bool isViewhostEvent = true);

    void sendHierarchy(const std::string& messageKey, bool blocking = false);

    rapidjson::Value buildDisplayedChildrenHierarchy(const apl::ComponentPtr& component, AplCoreViewhostMessage& message);

    /**
     * Process set of dirty components and send out dirty properties as required.
     * @param dirty dirty components set.
     */
    void processDirty(const std::set<apl::ComponentPtr>& dirty);

    /**
     * APL Core relies on operations to be performed in particular way.
     * Order and set of operations in this method should be preserved.
     * Order is the following:
     * * Update time and adjust TimeZone if required.
     * * Call **clearPending** method on RootConfig to give Core possibility to execute all pending actions and updates.
     * * Process requested events.      * * Process dirty properties.
     * * Check and set screenlock if required.
     */
    void coreFrameUpdate();

    /**
     * Sends an error message to the view host
     * @param message The message to send to the view hsot
     */
    void sendError(const std::string& message);

    /**
     * Get optional value from Json.
     * @param jsonNode json data
     * @param key Member name
     * @param defaultValue Default value
     * @return Value from json or default if not found.
     */
    double getOptionalValue(const rapidjson::Value& jsonNode, const std::string& key, double defaultValue);

    /**
     * Get optional value from Json.
     * @param jsonNode json data
     * @param key Member name
     * @param defaultValue Default value
     * @return Value from json or default if not found.
     */
    std::string getOptionalValue(
        const rapidjson::Value& jsonNode,
        const std::string& key,
        const std::string& defaultValue);

    /**
     * Get optional bool value from Json.
     * @param jsonNode json data
     * @param key Member name
     * @param defaultValue Default value
     * @return Value from json or default if not found.
     */
    bool getOptionalBool(const rapidjson::Value& jsonNode, const std::string& key, bool defaultValue);

    /**
     * Get optional integer value from Json.
     * @param jsonNode json data
     * @param key Member name
     * @param defaultValue Default value
     * @return Value from json or default if not found.
     */
    int getOptionalInt(const rapidjson::Value& jsonNode, const std::string& key, int defaultValue);

    /**
     * Gets a rect from Json and converts it to an apl::Rect
     * @param jsonNode json data
     * @return An apl rect with the values from the json
     */
    apl::Rect convertJsonToScaledRect(const rapidjson::Value& jsonNode);

    /**
     * Check if any errors returned from any of loaded datasources and report them.
     */
    void checkAndSendDataSourceErrors();

    /**
     * Serialize a rapidjson document node into a string.
     *
     * @param documentNode A logical node within a parsed JSON document which rapidjson understands.
     * @param[out] value The output parameter where the converted string will be assigned.
     */
    void serializeJSONValueToString(const rapidjson::Value& documentNode, std::string* value);

    /**
     * Update the latest configurationChange which is maintained at device runtime.
     *
     * @param configurationChange the new reported configurationChange.
     */
    void updateConfigurationChange(const apl::ConfigurationChange& configurationChange);

    rapidjson::Value getVisualContext(rapidjson::Document::AllocatorType& allocator);

    rapidjson::Value getDataSourceContext(rapidjson::Document::AllocatorType& allocator);

    void getVisualContext(const rapidjson::Value& payload);

    void getDataSourceContext(const rapidjson::Value& payload);

    bool registerRequestedExtensions();

    AplConfigurationPtr m_aplConfiguration;

    AplViewhostConfigPtr m_viewhostConfig;

    /// View host message type to handler map
    std::map<std::string, std::function<void(const rapidjson::Value&)>> m_messageHandlers;

    /// Shared pointer to the APL Content
    apl::ContentPtr m_Content;

    /// The APL presentation token for the currently rendered document
    std::string m_aplToken;

    /// The APL RootConfig object - received from the view host and used for generating the apl root context
    apl::RootConfig m_RootConfig;

    /// The APL Metrics object - received from the view host and used for generating the apl root context
    apl::Metrics m_Metrics;

    /**
     * The Viewport Size Specifications object - created on directive processing and passed to core in
     * order to calculate scaling.
     */
    std::vector<apl::ViewportSpecification> m_ViewportSizeSpecifications;

    /**
     * Scaling calculation object.
     */
    AplCoreMetricsPtr m_AplCoreMetrics;

    /// Pointer to the latest configuration change object.
    apl::ConfigurationChange m_ConfigurationChange = apl::ConfigurationChange();

    /// Pointer to the APL Root Context
    apl::RootContextPtr m_Root;

    /// Map of pending APL Core events
    std::map<int, apl::ActionRef> m_PendingEvents;

    /// The start time used to calculate the update time used by APL Core
    std::chrono::milliseconds m_StartTime;

    /// Screen lock flag
    bool m_ScreenLock;

    /// Next packet sequence number
    unsigned int m_SequenceNumber;

    /// The sequence number which a blockingSend is waiting for
    unsigned int m_replyExpectedSequenceNumber;

    /// Whether we are expecting a reply to a blockingSend
    bool m_blockingSendReplyExpected;

    /// The pending promise from a call to blockingSend
    std::promise<std::string> m_replyPromise;

    /// The mutex protecting blockingSend
    std::mutex m_blockingSendMutex;

    /// Pointer to ExtensionManager
    AplCoreExtensionManagerPtr m_extensionManager;

    /// Pointer to the active @c AplDocumentState to restore.
    AplDocumentStatePtr m_documentStateToRestore;

    alexaext::ExtensionProviderPtr m_extensionRegistrar;

    std::chrono::steady_clock::time_point m_renderingStart;

    std::list<std::shared_ptr<SupportedExtension>> m_supportedExtensions;

    std::shared_ptr<AplCoreAudioPlayerFactory> m_audioPlayerFactory;

    std::shared_ptr<AplCoreMediaPlayerFactory> m_mediaPlayerFactory;
};

using AplCoreConnectionManagerPtr = std::shared_ptr<AplCoreConnectionManager>;
using AplCoreConnectionManagerWPtr = std::weak_ptr<AplCoreConnectionManager>;

}  // namespace APLClient

#endif  // APL_CLIENT_LIBRARY_APL_CORE_CONNECTION_MANAGER_H_
