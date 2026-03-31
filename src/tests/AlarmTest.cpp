//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Alarm.h"
#include "../Network.h"
#include "../Config.h"

class SilentAlarm : public Alarm {
public:
    SilentAlarm(Network& network, Config& config) : Alarm(network, config) {}
protected:
    bool loadSound(sf::SoundBuffer&) override { return true; }
};

class AlarmTest : public ::testing::Test {
protected:
    void SetUp() override {
        network = new Network();
        config = new Config(*network);
        alarm = new SilentAlarm(*network, *config);
    }

    void TearDown() override {
        delete alarm;
        delete network;
        delete config;
    }
    Network* network;
    Config* config;
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