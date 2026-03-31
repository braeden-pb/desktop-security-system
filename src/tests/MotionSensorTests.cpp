//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Motion_Sensor.h"

class MotionSensorTests : public ::testing::Test {
protected:
    void SetUp() override {
        sensor = new Motion_Sensor();
    }

    void TearDown() override {
        delete sensor;
    }

    Motion_Sensor* sensor;
};

TEST_F(MotionSensorTests, InitiallyNotConnected) {
    EXPECT_FALSE(sensor->isConnected());
}

TEST_F(MotionSensorTests, ActivateSetsConnected) {
    sensor->activate();
    EXPECT_TRUE(sensor->isConnected());
}

TEST_F(MotionSensorTests, DisconnectSetsDisconnected) {
    sensor->activate();
    sensor->disconnect();
    EXPECT_FALSE(sensor->isConnected());
}

TEST_F(MotionSensorTests, InitiallyNoMotionDetected) {
    EXPECT_FALSE(sensor->detectMotion());
}

TEST_F(MotionSensorTests, OnMotionSetsMotionDetected) {
    sensor->onMotion();
    EXPECT_TRUE(sensor->detectMotion());
}

TEST_F(MotionSensorTests, GetSensitivityDefault) {
    EXPECT_EQ(sensor->getSensitivity(), 5);
}

TEST_F(MotionSensorTests, SetSensitivity) {
    sensor->setSensitivity(10);
    EXPECT_EQ(sensor->getSensitivity(), 10);
}

TEST_F(MotionSensorTests, GetNameReturnsCorrectName) {
    EXPECT_EQ(sensor->getName(), "Motion_Sensor");
}

TEST_F(MotionSensorTests, GetStatusDisconnected) {
    EXPECT_EQ(sensor->getStatus(), "Disconnected");
}

TEST_F(MotionSensorTests, GetStatusConnected) {
    sensor->activate();
    EXPECT_EQ(sensor->getStatus(), "Connected");
}