#include <gtest/gtest.h>
#include "../Alert.h"
#include "../SecuritySystem.h"

class AlertTest : public ::testing::Test {
protected:
    void SetUp() override {
        system = new SecuritySystem(true);
        alert = new Alert(*system);
    }

    void TearDown() override {
        delete alert;
        delete system;
    }

    SecuritySystem* system;
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