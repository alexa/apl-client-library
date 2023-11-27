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

#include <alexaext/alexaext.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "APLClient/Extensions/AplCoreExtensionManager.h"

using namespace ::testing;


namespace APLClient {

namespace Extensions {

namespace test {

static const std::string URI("testUri");

class MockAplCoreExtensionInterface : public APLClient::Extensions::AplCoreExtensionInterface {
public:
    MOCK_METHOD0(getUri, std::string());
    MOCK_METHOD0(getEnvironment, apl::Object());
    MOCK_METHOD0(getCommandDefinitions, std::list<apl::ExtensionCommandDefinition>());
    MOCK_METHOD0(getEventHandlers, std::list<apl::ExtensionEventHandler>());
    MOCK_METHOD0(getLiveDataObjects, std::unordered_map<std::string, apl::LiveObjectPtr>());
    MOCK_METHOD1(applySettings, void(const apl::Object& settings));
    MOCK_METHOD1(setEventHandler, void(std::shared_ptr<AplCoreExtensionEventHandlerInterface> eventHandler));

    MOCK_METHOD6(onExtensionEvent, void(const std::string& uri,
        const std::string& name,
        const apl::Object& source,
        const apl::Object& params,
        unsigned int event,
        std::shared_ptr<AplCoreExtensionEventCallbackResultInterface> resultCallback));
};

class MockAlexaextExtension : public alexaext::Extension {
public:
    MOCK_METHOD0(getURIs, const std::set<std::string>&());
    MOCK_METHOD2(createRegistration, rapidjson::Document(const std::string& uri, const rapidjson::Value& registrationRequest));
    MOCK_METHOD2(createRegistration, rapidjson::Document(const alexaext::ActivityDescriptor& activity, const rapidjson::Value& registrationRequest));
    MOCK_METHOD1(registerEventCallback, void(EventCallback callback));
    MOCK_METHOD1(registerEventCallback, void(EventActivityCallback&& callback));
    MOCK_METHOD1(registerLiveDataUpdateCallback, void(LiveDataUpdateCallback callback));
    MOCK_METHOD1(registerLiveDataUpdateCallback, void(LiveDataUpdateActivityCallback&& callback));
    MOCK_METHOD2(invokeCommand, bool(const std::string& uri, const rapidjson::Value& command));
    MOCK_METHOD2(invokeCommand, bool(const alexaext::ActivityDescriptor& activity, const rapidjson::Value& command));
    MOCK_METHOD2(onRegistered, void(const std::string& uri, const std::string& token));
    MOCK_METHOD1(onActivityRegistered, void(const alexaext::ActivityDescriptor& activity));
    MOCK_METHOD2(onUnregistered, void(const std::string& uri, const std::string& token));
    MOCK_METHOD1(onActivityUnregistered, void(const alexaext::ActivityDescriptor& activity));
    MOCK_METHOD2(updateComponent, bool(const std::string& uri, const rapidjson::Value& command));
    MOCK_METHOD2(updateComponent, bool(const alexaext::ActivityDescriptor& activity, const rapidjson::Value& command));
    MOCK_METHOD2(onResourceReady, void(const std::string& uri, const alexaext::ResourceHolderPtr& resourceHolder));
    MOCK_METHOD2(onResourceReady, void(const alexaext::ActivityDescriptor& activity, const alexaext::ResourceHolderPtr& resourceHolder));
    MOCK_METHOD1(onSessionStarted, void(const alexaext::SessionDescriptor& session));
    MOCK_METHOD1(onSessionEnded, void(const alexaext::SessionDescriptor& session));
    MOCK_METHOD1(onForeground, void(const alexaext::ActivityDescriptor& activity));
    MOCK_METHOD1(onBackground, void(const alexaext::ActivityDescriptor& activity));
    MOCK_METHOD1(onHidden, void(const alexaext::ActivityDescriptor& activity));
};

class AplCoreExtensionManagerTest : public ::testing::Test {
public:
    /// Set up the test harness for running a test.
    void SetUp() override;
    /// Clean up the test harness after running a test.
    void TearDown() override;

protected:
    /// Pointer to the @c AplCoreExtensionManager 
    std::shared_ptr<AplCoreExtensionManager> m_aplCoreExtensionManager;
    std::shared_ptr<MockAplCoreExtensionInterface> m_mockAplCoreExtensionInterface;
    std::shared_ptr<MockAlexaextExtension> m_mockAlexaextExtension;
};

void AplCoreExtensionManagerTest::SetUp() {
    m_mockAplCoreExtensionInterface = std::make_shared<NiceMock<MockAplCoreExtensionInterface>>();
    m_mockAlexaextExtension = std::make_shared<NiceMock<MockAlexaextExtension>>();
    m_aplCoreExtensionManager = std::make_shared<AplCoreExtensionManager>();
}

void AplCoreExtensionManagerTest::TearDown() {
    m_aplCoreExtensionManager.reset();
}


// 
// Legacy Extensions
//

// Add an extension
TEST_F(AplCoreExtensionManagerTest, AddLegacyExtension) {
    // Arrange
    EXPECT_CALL(*m_mockAplCoreExtensionInterface, getUri()).Times(3).WillOnce(Return(std::string()));

    // Act
    m_aplCoreExtensionManager->addExtension(m_mockAplCoreExtensionInterface);
}

// Add an extension, ensure m_UseAlexaExt reports as expected
TEST_F(AplCoreExtensionManagerTest, AddLegacyExtensionVerifyUseAlexaExt) {
    // Arrange
    EXPECT_CALL(*m_mockAplCoreExtensionInterface, getUri()).Times(3).WillOnce(Return(std::string()));

    // Act
    m_aplCoreExtensionManager->addExtension(m_mockAplCoreExtensionInterface);

    // Assert
    ASSERT_FALSE(m_aplCoreExtensionManager->useAlexaExt());
}

// Add a legacy extension, then add another legacy extension
TEST_F(AplCoreExtensionManagerTest, AddMultipleLegacyExtension) {
    // Arrange
    EXPECT_CALL(*m_mockAplCoreExtensionInterface, getUri()).Times(4)
        .WillOnce(Return(URI + "1"))
        .WillOnce(Return(URI + "1"))
        .WillOnce(Return(URI + "2"))
        .WillOnce(Return(URI + "2"));


    // Act
    m_aplCoreExtensionManager->addExtension(m_mockAplCoreExtensionInterface);
    m_aplCoreExtensionManager->addExtension(m_mockAplCoreExtensionInterface);
}

// Add a legacy extension, then try to add an AlexaExt
TEST_F(AplCoreExtensionManagerTest, AddLegacyExtensionThenAlexaExt) {
    // Arrange
    EXPECT_CALL(*m_mockAplCoreExtensionInterface, getUri()).Times(3).WillRepeatedly(Return(URI));
    std::set<std::string> URIset;
    URIset.insert(URI);
    EXPECT_CALL(*m_mockAlexaextExtension, getURIs()).Times(0);

    // Act
    m_aplCoreExtensionManager->addExtension(m_mockAplCoreExtensionInterface);
    m_aplCoreExtensionManager->addAlexaExtExtension(m_mockAlexaextExtension);

    // Assert
    ASSERT_FALSE(m_aplCoreExtensionManager->useAlexaExt());
}

// Get a registered extension
TEST_F(AplCoreExtensionManagerTest, GetLegacyExtension) {
    // Arrange
    EXPECT_CALL(*m_mockAplCoreExtensionInterface, getUri()).Times(4).WillRepeatedly(Return(std::string(URI)));

    // Act
    m_aplCoreExtensionManager->addExtension(m_mockAplCoreExtensionInterface);
    auto retrievedExtension = m_aplCoreExtensionManager->getExtension(URI);

    // Assert
    ASSERT_EQ(URI, retrievedExtension->getUri());
}

// Attempt to get an extension that doens't exist
TEST_F(AplCoreExtensionManagerTest, GetInvalidLegacyExtension) {
    // Act
    auto retrievedExtension = m_aplCoreExtensionManager->getExtension(URI);

    // Assert
    ASSERT_EQ(nullptr, retrievedExtension);
}

//
// AlexaExt Extensions
//

// Add an extension
TEST_F(AplCoreExtensionManagerTest, AddAlexaExt) {
    // Arrange
    std::set<std::string> URIset;
    URIset.insert(URI);
    EXPECT_CALL(*m_mockAlexaextExtension, getURIs()).Times(1).WillOnce(ReturnRef(URIset));

    // Act
    m_aplCoreExtensionManager->addAlexaExtExtension(m_mockAlexaextExtension);
}

// Add an extension, ensure m_UseAlexaExt reports as expected
TEST_F(AplCoreExtensionManagerTest, AddAlexaExtVerifyUseAlexaExt) {
    // Arrange
    std::set<std::string> URIset;
    URIset.insert(URI);
    EXPECT_CALL(*m_mockAlexaextExtension, getURIs()).Times(1).WillOnce(ReturnRef(URIset));

    // Act
    m_aplCoreExtensionManager->addAlexaExtExtension(m_mockAlexaextExtension);

    // Assert
    ASSERT_TRUE(m_aplCoreExtensionManager->useAlexaExt());
}
// Add an Alexaext, and then another AlexaExt
TEST_F(AplCoreExtensionManagerTest, AddAlexaExtThenAddAlexaExt) {
    // Arrange
    std::set<std::string> URIset;
    URIset.insert(URI);
    std::set<std::string> URIset1;
    URIset1.insert(URI + "1");
    EXPECT_CALL(*m_mockAlexaextExtension, getURIs()).Times(2).WillOnce(ReturnRef(URIset)).WillOnce(ReturnRef(URIset1));

    // Act
    m_aplCoreExtensionManager->addAlexaExtExtension(m_mockAlexaextExtension);
    m_aplCoreExtensionManager->addAlexaExtExtension(m_mockAlexaextExtension);

    // Assert
    ASSERT_TRUE(m_aplCoreExtensionManager->useAlexaExt());
}


// Get an extension
TEST_F(AplCoreExtensionManagerTest, GetAlexaExt) {
    // Arrange
    std::set<std::string> URIset;
    URIset.insert(URI);
    EXPECT_CALL(*m_mockAlexaextExtension, getURIs()).Times(3).WillRepeatedly(ReturnRef(URIset));

    // Act
    m_aplCoreExtensionManager->addAlexaExtExtension(m_mockAlexaextExtension);
    auto extension = m_aplCoreExtensionManager->getAlexaExtExtension(URI);

    //
    auto setElement = *(extension->getURIs().begin());
    ASSERT_EQ(1, extension->getURIs().size());
    ASSERT_EQ(URI, setElement);
}

// Add an AlexaExt extension, attempt to add a legacy extension
TEST_F(AplCoreExtensionManagerTest, AddAlexaExtThenLegacyExt) {
    // Arrange
    std::set<std::string> URIset;
    URIset.insert(URI);
    EXPECT_CALL(*m_mockAlexaextExtension, getURIs()).Times(1).WillOnce(ReturnRef(URIset));
    EXPECT_CALL(*m_mockAplCoreExtensionInterface, getUri()).Times(0).WillRepeatedly(Return(URI));

    // Act
    m_aplCoreExtensionManager->addAlexaExtExtension(m_mockAlexaextExtension);
    m_aplCoreExtensionManager->addExtension(m_mockAplCoreExtensionInterface);

    // Assert
    ASSERT_TRUE(m_aplCoreExtensionManager->useAlexaExt());
}


TEST_F(AplCoreExtensionManagerTest, template) {
}

} // namespace test
} // namespace Extensions
} // namespace APLClient