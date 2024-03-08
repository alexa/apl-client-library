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

#ifndef APL_CLIENT_LIBRARY_APL_SESSION_H_
#define APL_CLIENT_LIBRARY_APL_SESSION_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#pragma push_macro("DEBUG")
#undef DEBUG
#include <apl/utils/session.h>
#pragma pop_macro("DEBUG")
#pragma GCC diagnostic pop

namespace APLClient {

class AplSession : public apl::Session {
public:
    AplSession(bool isLogCommandEnabled);
    void write(const char *filename, const char *func, const char *value) override;
    void write(apl::LogCommandMessage&& message) override;

private:
    const apl::SessionPtr m_defaultSession = apl::makeDefaultSession();
    const bool m_isLogCommandEnabled;
    std::string getLogInfoFrom(const apl::LogCommandMessage& message);
};

}  // namespace APLClient

#endif  // APL_CLIENT_LIBRARY_APL_SESSION_H_