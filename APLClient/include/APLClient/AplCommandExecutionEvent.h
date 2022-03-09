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

#ifndef APLCLIENT_INCLUDE_APLCOMMANDEXECUTIONEVENT_H_
#define APLCLIENT_INCLUDE_APLCOMMANDEXECUTIONEVENT_H_

namespace APLClient {

/// Enumeration of APL Command Execution Events that could be sent from APLClient to runtime when processing commands.
enum class AplCommandExecutionEvent {
    /// APL Core Engine resolved the commands sequence.
    RESOLVED,

    /// APL Core Engine terminated the commands sequence.
    TERMINATED,

    /// APL Client/Core failed to parse/handle the commands sequence.
    FAILED
};

/**
 * This is a function to convert the @c AplCommandExecutionEvent to a string.
 */
inline std::string commandExecutionEventToString(const AplCommandExecutionEvent event) {
    switch (event) {
        case AplCommandExecutionEvent::RESOLVED:
            return "RESOLVED";
        case AplCommandExecutionEvent::TERMINATED:
            return "TERMINATED";
        case AplCommandExecutionEvent::FAILED:
            return "FAILED";
    }
    return "UNKNOWN";
}

}  // namespace APLClient

#endif  // APLCLIENT_INCLUDE_APLCOMMANDEXECUTIONEVENT_H_
