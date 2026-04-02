//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Alarm_PI.h"
#include "../NetworkServer.h"

class AlarmPITests : public ::testing::Test {
protected:
    void SetUp() override {
        server = new NetworkServer(0); // port 0 = any available port
        alarm = new Alarm_PI(*server);
    }

    void TearDown() override {
        delete alarm;
        delete server;
    }

    NetworkServer* server;
    Alarm_PI* alarm;
};

/**
 * @test Alarm should not be playing on construction.
 */
TEST_F(AlarmPITests, InitiallyNotPlaying) {
    EXPECT_FALSE(alarm->isPlaying());
}

/**
 * @test soundAlarm() should set alarm to playing state.
 */
TEST_F(AlarmPITests, SoundAlarmSetsPlayingTrue) {
    alarm->soundAlarm("alarm");
    EXPECT_TRUE(alarm->isPlaying());
    alarm->disableAlarm();
}

/**
 * @test disableAlarm() should stop the alarm.
 */
TEST_F(AlarmPITests, DisableAlarmSetsPlayingFalse) {
    alarm->soundAlarm("alarm");
    alarm->disableAlarm();
    EXPECT_FALSE(alarm->isPlaying());
}

/**
 * @test disableAlarm() when not playing should not crash.
 */
TEST_F(AlarmPITests, DisableWhenNotPlayingIsNoOp) {
    EXPECT_NO_THROW(alarm->disableAlarm());
}

/**
 * @test soundAlarm() called twice should not crash.
 */
TEST_F(AlarmPITests, SoundAlarmTwiceDoesNotCrash) {
    EXPECT_NO_THROW({
        alarm->soundAlarm("alarm");
        alarm->soundAlarm("alarm");
    });
    alarm->disableAlarm();
}

