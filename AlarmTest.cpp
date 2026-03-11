//
// Created by rushd on 3/10/26.
//
#include <gtest/gtest.h>
#include "Alarm.h"

TEST(AlarmTest, DefaultStatusIsInactive) {
    Alarm alarm;
    EXPECT_FALSE(alarm.getStatus());
}

TEST(AlarmTest, ActivateSetsStatusTrue) {
    Alarm alarm;
    alarm.activate();
    EXPECT_TRUE(alarm.getStatus());
}

TEST(AlarmTest, DeactivateSetsStatusFalse) {
    Alarm alarm;
    alarm.activate();
    alarm.deactivate();
    EXPECT_FALSE(alarm.getStatus());
}

TEST(AlarmTest, TestAlarmRunsWithoutCrash) {
    Alarm alarm;
    EXPECT_NO_THROW(alarm.testAlarm());
}