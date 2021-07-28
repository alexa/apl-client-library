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

#ifndef APLCLIENTSANDBOX_INCLUDE_APLMETRICSSTREAMSINK_H
#define APLCLIENTSANDBOX_INCLUDE_APLMETRICSSTREAMSINK_H

#include <stdint.h>

#include <chrono>
#include <map>
#include <memory>
#include <string>

//#include "APLClient/AplConfiguration.h"
#include "APLClient/Telemetry/AplMetricsSinkInterface.h"
#include "AplClientBridge.h"

class AplMetricsStreamSink : public APLClient::Telemetry::AplMetricsSinkInterface {
public:
    AplMetricsStreamSink(std::shared_ptr<AplClientBridge>);
    ~AplMetricsStreamSink() = default;

    std::string formatMetricLog(const std::string name, const std::string value);

    void reportTimer(const std::map<std::string, std::string>& metadata,
                     const std::string& name,
                     const std::chrono::nanoseconds& value);
    void reportCounter(const std::map<std::string, std::string>& metadata,
                       const std::string& name,
                       uint64_t value);
private:
    std::shared_ptr<AplClientBridge> mAplClientBridge;
};

using AplMetricsStreamSinkPtr = std::shared_ptr<AplMetricsStreamSink>;

#endif //APLCLIENTSANDBOX_INCLUDE_APLMETRICSSTREAMSINK_H
