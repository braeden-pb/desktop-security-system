/**
 * @file NetworkServer_Test.cpp
 * @brief Unit and integration tests for the NetworkServer class.
 * @author Braeden Patierno-Barker
 * @date 2026-03-31
 */

#include <gtest/gtest.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "../NetworkServer.h"

/**
 * @brief Test fixture for NetworkServer.
 * * Handles starting the server on a non-standard port (5001) to avoid
 * conflicts with the live system.
 */
class NetworkServer_Test : public ::testing::Test {
protected:
    int testPort = 5001;
    std::unique_ptr<NetworkServer> server;

    void SetUp() override {
        server = std::make_unique<NetworkServer>(testPort);
    }

    void TearDown() override {
        server->stop();
        server.reset();
    }

    /**
     * @brief Helper to create a basic TCP client and connect to the server.
     * @return The client socket file descriptor, or -1 on failure.
     */
    int createTestClient() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(testPort);
        inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

        if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            close(sock);
            return -1;
        }
        return sock;
    }
};

/**
 * @brief Verifies that the server can bind and start listening on the port.
 */
TEST_F(NetworkServer_Test, ServerStartsAndReportsCorrectPort) {
    EXPECT_TRUE(server->start());
    EXPECT_EQ(server->getPort(), testPort);
}

/**
 * @brief Verifies that the server correctly identifies its own IP.
 */
TEST_F(NetworkServer_Test, ServerRetrievesIPAddress) {
    std::string ip = server->getIP();
    EXPECT_FALSE(ip.empty());
    // Basic check to see if it looks like an IP (contains dots or is 'localhost')
    EXPECT_TRUE(ip.find('.') != std::string::npos || ip.find(':') != std::string::npos);
}

/**
 * @brief Verifies the command callback is triggered when the client sends a packet.
 */
TEST_F(NetworkServer_Test, TriggersOnCommandWhenPacketReceived) {
    bool commandReceived = false;
    Command receivedCmd = Command::None;

    server->onCommand([&](Command cmd, const std::vector<uint8_t>& payload) {
        commandReceived = true;
        receivedCmd = cmd;
    });

    server->start();

    int clientSock = createTestClient();
    ASSERT_GT(clientSock, 0);

    PacketHeader header{};
    header.system = System::Camera;
    header.command = Command::TakePhoto;
    header.payloadSize = 0;

    send(clientSock, &header, sizeof(header), 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_TRUE(commandReceived);
    EXPECT_EQ(receivedCmd, Command::TakePhoto);

    close(clientSock);
}

/**
 * @brief Verifies that sendPacket correctly transmits data to a connected client.
 */
TEST_F(NetworkServer_Test, TransmitsPacketToClient) {
    server->start();
    int clientSock = createTestClient();
    ASSERT_GT(clientSock, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    PacketHeader sendHeader{};
    sendHeader.system = System::Motion;
    sendHeader.command = Command::MotionDetected;
    sendHeader.payloadSize = 0;

    server->sendPacket(sendHeader, {});

    PacketHeader recvHeader{};
    int n = recv(clientSock, &recvHeader, sizeof(recvHeader), MSG_WAITALL);

    EXPECT_EQ(n, sizeof(PacketHeader));
    EXPECT_EQ(recvHeader.command, Command::MotionDetected);

    close(clientSock);
}

/**
 * @brief Verifies the onDisconnect callback is triggered when the socket is closed.
 */
TEST_F(NetworkServer_Test, TriggersOnDisconnectCallback) {
    bool disconnected = false;
    server->onDisconnect([&]() {
        disconnected = true;
    });

    server->start();
    int clientSock = createTestClient();
    ASSERT_GT(clientSock, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    close(clientSock);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(disconnected);
}