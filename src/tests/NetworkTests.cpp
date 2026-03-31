//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Network.h"

class NetworkTests : public ::testing::Test {
protected:
    void SetUp() override {
        network = new Network();
    }

    void TearDown() override {
        delete network;
    }

    Network* network;
};

TEST_F(NetworkTests, InitiallyNotConnected) {
    EXPECT_FALSE(network->isConnected());
}

TEST_F(NetworkTests, ConnectToInvalidIPReturnsFalse) {
    EXPECT_FALSE(network->connect("0.0.0.0", 9999));
}

TEST_F(NetworkTests, ConnectToLocalhostNoServerReturnsFalse) {
    EXPECT_FALSE(network->connect("127.0.0.1", 9999));
}

TEST_F(NetworkTests, DisconnectWhenNotConnectedIsNoOp) {
    EXPECT_NO_THROW(network->disconnect());
}

TEST_F(NetworkTests, SendWhenNotConnectedDoesNotCrash) {
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::TakePhoto;
    header.payloadSize = 0;
    EXPECT_NO_THROW(network->send(header, {}));
}

TEST_F(NetworkTests, StopReceivingWhenNotStartedDoesNotCrash) {
    EXPECT_NO_THROW(network->stopReceiving());
}

TEST_F(NetworkTests, RegisterPacketHandlerDoesNotCrash) {
    EXPECT_NO_THROW(network->onPacket(System::Camera, [](Command, const std::vector<uint8_t>&) {}));
}