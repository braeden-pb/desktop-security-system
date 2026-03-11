#include <gtest/gtest.h>
#include "Alarm.h"

// Bypasses sound loading so tests work without audio hardware
class SilentAlarm : public Alarm {
protected:
    bool loadSound(sf::SoundBuffer&) override { return true; }
};

TEST(AlarmTest, DefaultStatusIsInactive) {
    Alarm alarm;  // fine as-is, never calls loadSound
    EXPECT_FALSE(alarm.getStatus());
}

TEST(AlarmTest, ActivateSetsStatusTrue) {
    SilentAlarm alarm;
    alarm.activate();
    EXPECT_TRUE(alarm.getStatus());
}

TEST(AlarmTest, DeactivateSetsStatusFalse) {
    SilentAlarm alarm;
    alarm.activate();
    alarm.deactivate();
    EXPECT_FALSE(alarm.getStatus());
}

TEST(AlarmTest, TestAlarmRunsWithoutCrash) {
    SilentAlarm alarm;
    EXPECT_NO_THROW(alarm.testAlarm());
}