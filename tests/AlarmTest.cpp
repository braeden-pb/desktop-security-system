#include <gtest/gtest.h>
#include "../Alarm.h"
#include <thread>


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
    alarm.activate();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    alarm.deactivate();
    EXPECT_NO_THROW(alarm.testAlarm());
}