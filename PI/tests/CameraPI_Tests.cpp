/**
* @file Camera_PI_Test.cpp
 * @brief Unit tests for the Camera_PI class using real object instances.
 * @author Braeden Patierno-Barker
 * @date 2026-03-31
 */

#include <gtest/gtest.h>
#include "../Camera_PI.h"
#include "../NetworkServer.h"
#include "../../Shared/Protocol.h"

/**
 * @brief Test fixture for Camera_PI.
 * Matches the pattern used in Alarm_PI and Motion_Sensor_PI tests.
 */
class CameraPI_Tests : public ::testing::Test {
protected:
    void SetUp() override {
        server = new NetworkServer(0);
        camera = new Camera_PI(*server);
    }

    void TearDown() override {
        camera->stopStreaming();
        delete camera;
        delete server;
    }

    NetworkServer* server;
    Camera_PI* camera;
};

/**
 * @test Verifies that capturePhoto returns an empty string if hardware is missing.
 * @details On non-Pi hardware (like WSL), the libcamera init will fail,
 * returning an empty path.
 */
TEST_F(CameraPI_Tests, CaptureFailsWithoutHardware) {
    std::string path = camera->capturePhoto();
    EXPECT_EQ(path, "");
}

/**
 * @test Verifies that stopping a non-existent stream does not crash.
 */
TEST_F(CameraPI_Tests, StopStreamingWhenNotStreamingIsSafe) {
    EXPECT_NO_THROW(camera->stopStreaming());
}

/**
 * @test Ensures starting a stream multiple times is handled gracefully.
 */
TEST_F(CameraPI_Tests, DoubleStartStreamingDoesNotCrash) {
    EXPECT_NO_THROW({
        camera->startStreaming();
        camera->startStreaming();
    });
    camera->stopStreaming();
}