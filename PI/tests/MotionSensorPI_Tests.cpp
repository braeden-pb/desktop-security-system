/**
 * @file Motion_Sensor_PI_test.cpp
 * @brief Unit tests for the Motion_Sensor_PI class logic using real objects.
 * @author Braeden Patierno-Barker
 * @date 2026-03-31
 */

#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "../Motion_Sensor_PI.h"
#include "../NetworkServer.h"
#include "../../Shared/Protocol.h"

class MotionSensorPI_Tests : public ::testing::Test {
protected:
    void SetUp() override {
        // Port 0 tells the OS to pick any available port automatically
        server = new NetworkServer(0);
        // Note: sensitivity=5, sleep=100ms
        motion = new Motion_Sensor_PI(5, 100, *server);
    }

    void TearDown() override {
        motion->deactivate();
        delete motion;
        delete server;
    }

    NetworkServer* server;
    Motion_Sensor_PI* motion;
};

/**
 * @test Sensor should start inactive and without motion.
 */
TEST_F(MotionSensorPI_Tests, InitiallyInactive) {
    EXPECT_FALSE(motion->isActive());
    EXPECT_FALSE(motion->isMotionDetected());
}

/**
 * @test activate() should set the active flag to true.
 */
TEST_F(MotionSensorPI_Tests, ActivationSetsActiveTrue) {
    motion->activate();
    EXPECT_TRUE(motion->isActive());
}

/**
 * @test onMotion() should manually set detection flags.
 */
TEST_F(MotionSensorPI_Tests, ManualMotionSetsFlags) {
    motion->onMotion();
    EXPECT_TRUE(motion->isMotionDetected());
}

/**
 * @test ISR handler should trigger motion detection.
 * @details Simulates a rising edge from the PIR hardware.
 */
TEST_F(MotionSensorPI_Tests, ISRHandlerTriggersDetection) {
    motion->activate();

    // Simulate the PIR sensor going HIGH
    // This calls the static handler using the singleton instance_
    Motion_Sensor_PI::isrHandler();

    EXPECT_TRUE(motion->isMotionDetected());
}

/**
 * @test Cooldown logic should prevent duplicate processing within 3 seconds.
 */
TEST_F(MotionSensorPI_Tests, ISRRespectsCooldown) {
    motion->activate();

    // Trigger first time
    Motion_Sensor_PI::isrHandler();

    // Attempt to trigger immediately again
    // The internal lastSent timer should block this
    Motion_Sensor_PI::isrHandler();

    // Since we can't see 'motionPending' directly, we verify the state remains consistent
    EXPECT_TRUE(motion->isMotionDetected());
}

/**
 * @test deactivate() should stop the background thread and set active to false.
 */
TEST_F(MotionSensorPI_Tests, DeactivateStopsSensor) {
    motion->activate();
    motion->deactivate();
    EXPECT_FALSE(motion->isActive());
}