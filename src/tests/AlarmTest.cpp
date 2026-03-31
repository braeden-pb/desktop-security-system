#include <gtest/gtest.h>
#include "../Alarm.h"
#include "../Network.h"

class SilentAlarm : public Alarm {
public:
    SilentAlarm(Network& network) : Alarm(network) {}
protected:
    bool loadSound(sf::SoundBuffer&) override { return true; }
};

class AlarmTest : public ::testing::Test {
protected:
    void SetUp() override {
        network = new Network();
        alarm = new SilentAlarm(*network);
    }
    void TearDown() override {
        delete alarm;
        delete network;
    }
    Network* network;
    SilentAlarm* alarm;
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