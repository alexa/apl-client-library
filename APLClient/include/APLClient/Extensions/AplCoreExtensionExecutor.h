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
#ifndef APLCLIENT_EXTENSIONS_APLCOREEXTENSIONEXECUTOR_H
#define APLCLIENT_EXTENSIONS_APLCOREEXTENSIONEXECUTOR_H

#include <mutex>

#include <alexaext/alexaext.h>

namespace APLClient {

namespace Extensions {

class AlexaExtExtensionExecutor : public alexaext::Executor {
public:
    AlexaExtExtensionExecutor() = default;
    ~AlexaExtExtensionExecutor() override = default;
    bool enqueueTask(Task task) override;

private:
    std::recursive_mutex m_Mutex;
};

using AlexaExtExtensionExecutorPtr = std::shared_ptr<AlexaExtExtensionExecutor>;

} // namespace Extensions
} // namespace APLClient 

#endif  // APLCLIENT_EXTENSIONS_APLCOREEXTENSIONEXECUTOR_H
