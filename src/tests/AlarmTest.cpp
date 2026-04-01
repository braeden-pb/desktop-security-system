//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Alarm.h"
#include "../Network.h"
#include "../Config.h"

// Remove SilentAlarm entirely and just use Alarm directly
class AlarmTest : public ::testing::Test {
protected:
    void SetUp() override {
        network = new Network();
        config = new Config(*network);
        alarm = new Alarm(*network, *config);
    }
    void TearDown() override {
        delete alarm;
        delete config;
        delete network;
    }
    Network* network;
    Config* config;
    Alarm* alarm;
};

TEST_F(AlarmTest, DefaultStatusIsInactive) {
    EXPECT_FALSE(alarm->getStatus());
}

TEST_F(AlarmTest, ActivateSetsStatusTrue) {
    alarm->activate();
    EXPECT_TRUE(alarm->getStatus());
}

TEST_F(AlarmTest, DeactivateSetsStatusFalse) {
    alarm->activate();
    alarm->deactivate();
    EXPECT_FALSE(alarm->getStatus());
}

TEST_F(AlarmTest, TestAlarmRunsWithoutCrash) {
    EXPECT_NO_THROW(alarm->activate());
}