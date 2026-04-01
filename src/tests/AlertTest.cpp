//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Alert.h"
#include "../SecuritySystem.h"

class AlertTest : public ::testing::Test {
protected:
    void SetUp() override {
        system = new SecuritySystem(true);
        event = new Event("2026-03-06 15:00", "Motion detected");
        alert = new Alert(*event, *system);
    }

    void TearDown() override {
        delete alert;
        delete event;
        delete system;
    }

    SecuritySystem* system;
    Event* event;
    Alert* alert;
};

TEST_F(AlertTest, SendAlertWithoutCrash) {
    EXPECT_NO_THROW(alert->sendAlert());
}

TEST_F(AlertTest, DiscardRunsWithoutCrash) {
    EXPECT_NO_THROW(alert->discard());
}

TEST_F(AlertTest, MotionDetectedRunsWithoutCrash) {
    EXPECT_NO_THROW(alert->motionDetected());
}

TEST_F(AlertTest, UpdateRunsWithoutCrash) {
    EXPECT_NO_THROW(alert->update("Motion detected"));
}