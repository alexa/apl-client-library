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

#include "APLClient/AplSession.h"

#include <gtest/gtest.h>
#include <fstream>

namespace APLClient {
namespace test {

using namespace ::testing;

static const std::string TEXT = "test text";
static const std::string ARGUMENTS = "test arguments";
static const std::string ORIGIN = "test origin";

class AplSessionTest : public ::testing::Test {
public:
    void SetUp() override;
    std::string CaptureLog(apl::LogCommandMessage&& message);
    bool FindText(std::string text, std::string logContents);

protected:
    std::shared_ptr<AplSession> m_aplSession;
};

void AplSessionTest::SetUp() {
    m_aplSession = std::make_shared<AplSession>(true);
}

std::string AplSessionTest::CaptureLog(apl::LogCommandMessage&& message) {
    // Redirect stdout to a file
    std::string logFile = "CapturedLogCommand.txt";
    freopen(logFile.c_str(),"w", stdout);
    
    // Write the log message
    m_aplSession->write(std::move(message));

    // Redirect stdout back to terminal
    freopen("/dev/tty","w", stdout);

    // Read log file
    std::ifstream in(logFile);
    std::string logContents((std::istreambuf_iterator<char>(in)), 
                    std::istreambuf_iterator<char>());

    // Close and delete the temp log file
    in.close();
    std::remove(logFile.c_str());

    return logContents;
}

bool AplSessionTest::FindText(std::string text, std::string logContents) {
    return logContents.find(text) != std::string::npos;
}

TEST_F(AplSessionTest, WriteLogCommandMessage) {
     apl::LogCommandMessage message {
        TEXT,
        apl::LogLevel::kInfo,
        apl::Object(ARGUMENTS),
        apl::Object(ORIGIN)
    };

    std::string logContents = CaptureLog(std::move(message));

    EXPECT_TRUE(FindText(TEXT, logContents));
    EXPECT_TRUE(FindText(ARGUMENTS, logContents));
    EXPECT_TRUE(FindText(ARGUMENTS, logContents));
}

TEST_F(AplSessionTest, WriteLogCommandMessageWithoutArguments) {
     apl::LogCommandMessage message {
        TEXT,
        apl::LogLevel::kInfo,
        NULL,
        apl::Object(ORIGIN)
    };

    std::string logContents = CaptureLog(std::move(message));

    EXPECT_TRUE(FindText(TEXT, logContents));
    EXPECT_FALSE(FindText(ARGUMENTS, logContents));
    EXPECT_TRUE(FindText(ORIGIN, logContents));
}

TEST_F(AplSessionTest, WriteLogCommandMessageWithoutOrigin) {
     apl::LogCommandMessage message {
        TEXT,
        apl::LogLevel::kInfo,
        apl::Object(ARGUMENTS),
        NULL
    };

    std::string logContents = CaptureLog(std::move(message));

    EXPECT_TRUE(FindText(TEXT, logContents));
    EXPECT_TRUE(FindText(ARGUMENTS, logContents));
    EXPECT_FALSE(FindText(ORIGIN, logContents));
}

TEST_F(AplSessionTest, WriteLogCommandMessageWhenDisabled) {
    m_aplSession = std::make_shared<AplSession>(false);

     apl::LogCommandMessage message {
        TEXT,
        apl::LogLevel::kInfo,
        apl::Object(ARGUMENTS),
        apl::Object(ORIGIN)
    };

    std::string logContents = CaptureLog(std::move(message));

    EXPECT_FALSE(FindText(TEXT, logContents));
    EXPECT_FALSE(FindText(ARGUMENTS, logContents));
    EXPECT_FALSE(FindText(ORIGIN, logContents));
}

TEST_F(AplSessionTest, WriteLogWithFilenameFuncAndValue) {
    std::string logFile = "CapturedLogCommand.txt";
    freopen(logFile.c_str(),"w", stdout);
    
    m_aplSession->write("filename", "function", "test value");

    freopen("/dev/tty","w", stdout);

    std::ifstream in(logFile);
    std::string logContents((std::istreambuf_iterator<char>(in)), 
                    std::istreambuf_iterator<char>());

    in.close();
    std::remove(logFile.c_str());

    EXPECT_TRUE(FindText("test value", logContents));
}

} // namespace test
} // namespace APLClient