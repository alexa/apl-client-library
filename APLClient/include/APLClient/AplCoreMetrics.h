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

#ifndef APLCLIENT_APL_APLCOREMETRICS_H
#define APLCLIENT_APL_APLCOREMETRICS_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#pragma push_macro("DEBUG")
#pragma push_macro("TRUE")
#pragma push_macro("FALSE")
#undef DEBUG
#undef TRUE
#undef FALSE
#include <apl/apl.h>
#include <apl/scaling/metricstransform.h>
#pragma pop_macro("DEBUG")
#pragma pop_macro("TRUE")
#pragma pop_macro("FALSE")
#pragma GCC diagnostic pop

namespace APLClient {

/**
 * Extends apl MetricsTransform to transform units between viewhost and core dimensions
 */
class AplCoreMetrics : public apl::MetricsTransform {
public:
    explicit AplCoreMetrics(apl::Metrics& metrics) : MetricsTransform(metrics) {
    }
    AplCoreMetrics(apl::Metrics& metrics, apl::ScalingOptions& options) : MetricsTransform(metrics, options) {
    }

    virtual ~AplCoreMetrics() = default;

    /**
     * Converts dp units into px units
     * @param value dp unit
     * @return px unit
     */
    float toViewhost(float value) const override;

    /**
     * Converts px units into dp units
     * @param value px unit
     * @return dp unit
     */
    float toCore(float value) const override;

    /**
     * Return the viewport width in pixels
     * @return pixel width
     */
    float getViewhostWidth() const override;

    /**
     * Return the viewport height in pixels
     * @return pixel height
     */
    float getViewhostHeight() const override;

    /**
     * Converts px from VH to Core based on Scale.
     * @param value
     * @return
     */
    float toCorePixel(float value);
};

using AplCoreMetricsPtr = std::shared_ptr<AplCoreMetrics>;

}  // namespace APLClient

#endif  // APLCLIENT_APL_APLCOREMETRICS_H