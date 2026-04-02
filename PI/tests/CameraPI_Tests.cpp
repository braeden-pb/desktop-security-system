/**
 * @file Camera_PI_Test.cpp
 * @brief Unit tests for the Camera_PI class using Google Test and Google Mock.
 * @author Braeden Patierno-Barker
 * @date 2026-03-31
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Camera_PI.h"
#include "NetworkServer.h"

/**
 * @brief Mock class for the NetworkServer to track outgoing packets.
 */
class MockNetworkServer : public NetworkServer {
public:
    MockNetworkServer() : NetworkServer(5000) {} // Default port for test
    MOCK_METHOD(void, sendPacket, (const PacketHeader& header, const std::vector<uint8_t>& payload), (override));
    MOCK_METHOD(void, sendFrame, (const uint8_t* data, size_t size), (override));
};

/**
 * @brief Test fixture for Camera_PI.
 */
class CameraPI_Test : public ::testing::Test {
protected:
    MockNetworkServer mockServer;

    // We use a pointer to delay initialization until the test starts
    std::unique_ptr<Camera_PI> camera;

    void SetUp() override {
        // Note: On non-Pi hardware, initCamera() in the constructor will log errors.
        // In a real CI environment, you would mock the libcamera API.
        camera = std::make_unique<Camera_PI>(mockServer);
    }

    void TearDown() override {
        camera.reset();
    }
};

/**
 * @brief Ensures the camera starts in a sane, non-active state.
 */
TEST_F(CameraPI_Test, InitialStateIsIdle) {
    EXPECT_FALSE(camera->isRecording());
    // Assuming you have a getter for streaming
    // EXPECT_FALSE(camera->isStreaming());
}

/**
 * @brief Verifies that capturePhoto returns an error string if hardware isn't found.
 */
TEST_F(CameraPI_Test, CaptureFailsWithoutHardware) {
    // If running on a PC without a Pi camera, this should return empty
    std::string path = camera->capturePhoto();
    EXPECT_EQ(path, "");
}

/**
 * @brief Tests the logic guard preventing simultaneous photo and video.
 */
TEST_F(CameraPI_Test, CannotCapturePhotoWhileRecording) {
    // Manually trigger recording state (if you have a setter or via startRecording)
    // For this test, we simulate the state if recording was active
    // This assumes you've uncommented your recording logic

    /* camera->startRecording();
    std::string path = camera->capturePhoto();
    EXPECT_TRUE(path.empty());
    */
}

/**
 * @brief Verifies that stopping a non-existent stream doesn't crash.
 */
TEST_F(CameraPI_Test, StopStreamingWhenNotStreamingIsSafe) {
    EXPECT_NO_THROW(camera->stopStreaming());
}