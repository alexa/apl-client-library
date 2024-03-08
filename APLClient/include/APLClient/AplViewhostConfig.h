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

#ifndef APL_CLIENT_LIBRARY_VIEWHOST_CONFIG_H_
#define APL_CLIENT_LIBRARY_VIEWHOST_CONFIG_H_

#include <memory>

#include "Telemetry/AplMetricsRecorderInterface.h"

namespace APLClient {

enum class AnimationQuality {
    NONE,
    SLOW,
    NORMAL
};

/**
 * AplViewhostConfig is a class that holds configuration for the APL Viewhost.
 * This shares the same configuration to create the APLWSRenderer on the JS side.
 */
class AplViewhostConfig final {
public:
    AplViewhostConfig() = default;
    ~AplViewhostConfig() = default;

    AplViewhostConfig& viewportWidth(unsigned int width);
    AplViewhostConfig& viewportHeight(unsigned int height);
    AplViewhostConfig& viewportDpi(unsigned int dpi);
    AplViewhostConfig& viewportShape(const std::string& shape);
    AplViewhostConfig& isRounded(bool isRounded);
    AplViewhostConfig& viewportMode(const std::string& mode);

    AplViewhostConfig& agentName(const std::string& name);
    AplViewhostConfig& agentVersion(const std::string& version);
    AplViewhostConfig& allowOpenUrl(bool allow);
    AplViewhostConfig& disallowVideo(bool disallow);
    AplViewhostConfig& disallowEditText(bool disallow);
    AplViewhostConfig& disallowDialog(bool disallow);
    AplViewhostConfig& scrollCommandDuration(unsigned int milliseconds);
    AplViewhostConfig& animationQuality(const AnimationQuality& quality);
    AplViewhostConfig& isLogCommandEnabled(bool isLogCommandEnabled);

    unsigned int viewportWidth() const;
    unsigned int viewportHeight() const;
    unsigned int viewportDpi() const;
    std::string viewportShape() const;
    bool isRounded() const;
    std::string viewportMode() const;

    std::string agentName() const;
    std::string agentVersion() const;
    bool allowOpenUrl() const;
    bool disallowVideo() const;
    bool disallowEditText() const;
    bool disallowDialog() const;
    unsigned int scrollCommandDuration() const;
    AnimationQuality animationQuality() const;
    bool isLogCommandEnabled() const;

private:
    unsigned int m_viewportWidth = 0;
    unsigned int m_viewportHeight = 0;
    unsigned int m_viewportDpi = 160;
    std::string m_viewportShape = "RECTANGLE";
    bool m_isRounded = false;
    std::string m_viewportMode = "HUB";

    std::string m_agentName = "APLClientDefault";
    std::string m_agentVersion = "1.0.0";
    bool m_allowOpenUrl = false;
    bool m_disallowVideo = false;
    bool m_disallowEditText = false;
    bool m_disallowDialog = false;
    unsigned int m_scrollCommandDuration = 1000;
    AnimationQuality m_animationQuality = AnimationQuality::NORMAL;
    bool m_isLogCommandEnabled = false;
};

using AplViewhostConfigPtr = std::shared_ptr<AplViewhostConfig>;

} // namespace APLClient

#endif //APL_CLIENT_LIBRARY_VIEWHOST_CONFIG_H_
