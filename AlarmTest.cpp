//
// Created by rushd on 3/10/26.
//
#include <gtest/gtest.h>
#include "Alarm.h"

// Test that a newly created alarm starts inactive
TEST(AlarmTest, DefaultStatusIsInactive) {
    Alarm alarm;
    EXPECT_FALSE(alarm.getStatus());
}

// Test that activate() changes the alarm status to active
TEST(AlarmTest, ActivateSetsStatusTrue) {
    Alarm alarm;
    alarm.activate();
    EXPECT_TRUE(alarm.getStatus());
}

// Test that deactivate() changes the alarm status back to inactive
TEST(AlarmTest, DeactivateSetsStatusFalse) {
    Alarm alarm;
    alarm.activate();
    alarm.deactivate();
    EXPECT_FALSE(alarm.getStatus());
}

// Test that activate(durationSec) also sets the alarm to active
TEST(AlarmTest, TimedActivateSetsStatusTrue) {
    Alarm alarm;
    alarm.activate(1);
    EXPECT_TRUE(alarm.getStatus());
}

// Test that testAlarm() runs without throwing an exception
TEST(AlarmTest, TestAlarmRunsWithoutCrash) {
    Alarm alarm;
    EXPECT_NO_THROW(alarm.testAlarm());
}
TEST(AlarmTest, RepeatedDeactivateKeepsAlarmInactive) {
    Alarm alarm;
    alarm.deactivate();
    EXPECT_FALSE(alarm.getStatus());
}