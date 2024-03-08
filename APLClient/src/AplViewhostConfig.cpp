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

#include "APLClient/AplViewhostConfig.h"

namespace APLClient {

AplViewhostConfig&
AplViewhostConfig::viewportWidth(unsigned int width) {
    m_viewportWidth = width;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::viewportHeight(unsigned int height) {
    m_viewportHeight = height;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::viewportDpi(unsigned int dpi) {
    m_viewportDpi = dpi;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::viewportShape(const std::string& shape) {
    m_viewportShape = shape;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::isRounded(bool isRounded) {
    m_isRounded = isRounded;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::viewportMode(const std::string& mode) {
    m_viewportMode = mode;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::agentName(const std::string& name) {
    m_agentName = name;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::agentVersion(const std::string& version) {
    m_agentVersion = version;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::allowOpenUrl(bool allow) {
    m_allowOpenUrl = allow;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::disallowVideo(bool disallow) {
    m_disallowVideo = disallow;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::disallowEditText(bool disallow) {
    m_disallowEditText = disallow;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::disallowDialog(bool disallow) {
    m_disallowDialog = disallow;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::scrollCommandDuration(unsigned int milliseconds) {
    m_scrollCommandDuration = milliseconds;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::animationQuality(const AnimationQuality& quality) {
    m_animationQuality = quality;
    return *this;
}

AplViewhostConfig&
AplViewhostConfig::isLogCommandEnabled(bool isLogCommandEnabled) {
    m_isLogCommandEnabled = isLogCommandEnabled;
    return *this;
}

unsigned int
AplViewhostConfig::viewportWidth() const {
    return m_viewportWidth;
}

unsigned int
AplViewhostConfig::viewportHeight() const {
    return m_viewportHeight;
}

unsigned int
AplViewhostConfig::viewportDpi() const {
    return m_viewportDpi;
}

std::string
AplViewhostConfig::viewportShape() const {
    return m_viewportShape;
}

bool
AplViewhostConfig::isRounded() const {
    return m_isRounded;
}

std::string
AplViewhostConfig::viewportMode() const {
    return m_viewportMode;
}

std::string
AplViewhostConfig::agentName() const {
    return m_agentName;
}

std::string
AplViewhostConfig::agentVersion() const {
    return m_agentVersion;
}

bool
AplViewhostConfig::allowOpenUrl() const {
    return m_allowOpenUrl;
}

bool
AplViewhostConfig::disallowVideo() const {
    return m_disallowVideo;
}

bool
AplViewhostConfig::disallowEditText() const {
    return m_disallowEditText;
}

bool
AplViewhostConfig::disallowDialog() const {
    return m_disallowDialog;
}

unsigned int
AplViewhostConfig::scrollCommandDuration() const {
    return m_scrollCommandDuration;
}

AnimationQuality
AplViewhostConfig::animationQuality() const {
    return m_animationQuality;
}

bool
AplViewhostConfig::isLogCommandEnabled() const {
    return m_isLogCommandEnabled;
}

} // namespace APLClient