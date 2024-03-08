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

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "APLClient/AplSession.h"

namespace APLClient {

AplSession::AplSession(bool isLogCommandEnabled)
    : apl::Session(),
    m_isLogCommandEnabled(isLogCommandEnabled) {
}

void AplSession::write(const char *filename, const char *func, const char *value) { 
    m_defaultSession->write(filename, func, value); 
}

void AplSession::write(apl::LogCommandMessage&& message) {
    if (m_isLogCommandEnabled) {
        std::string info = getLogInfoFrom(message);

        apl::LoggerFactory::instance().getLogger(message.level, "Log", "Command")
            .session(*this)
            .log("%s %s", message.text.c_str(), info.c_str());
    }
}

std::string AplSession::getLogInfoFrom(const apl::LogCommandMessage& message) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    rapidjson::Document serializer;

    writer.StartObject();

    // Add arguments array if it's not empty
    if (!message.arguments.empty()) {
        writer.Key("arguments");
        rapidjson::Value serializedArguments = message.arguments.serialize(serializer.GetAllocator());
        serializedArguments.Accept(writer); // Serialize into the writer directly
    }

    // Add origin object if it's not empty
    if (!message.origin.empty()) {
        writer.Key("origin");
        rapidjson::Value serializedOrigin = message.origin.serialize(serializer.GetAllocator());
        serializedOrigin.Accept(writer);
    }

    writer.EndObject();

    return buffer.GetString();
}

}  // namespace APLClient