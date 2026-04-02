//
// Created by Mitchell on 2026-03-10.
//

#include <filesystem>
#include <gtest/gtest.h>
#include "../Config.h"
#include "../Network.h"
#include <fstream>

class ConfigTests : public ::testing::Test {
protected:
    void SetUp() override {
        network = new Network();
        configBase = new Config(*network);
        password = configBase->getPassword();
        photosPer = configBase->getPhotosPer();
        seconds = configBase->getSeconds();
        captureMode = configBase->getCaptureMode();
    }
    void TearDown() override {
        configBase->setCaptureMode(captureMode);
        configBase->setPassword(password);
        configBase->setPhotosPer(photosPer);
        configBase->setSeconds(seconds);
        configBase->writeToFile();
        delete configBase;
        delete network;
    }
    std::string password;
    int photosPer;
    int seconds;
    bool captureMode;
    Network* network;
    Config* configBase;
};

TEST_F(ConfigTests, ReadFile) {
    remove("config.json");
    configBase->setCaptureMode(true);
    configBase->setPassword("basepassword");
    configBase->setPhotosPer(90);
    configBase->setSeconds(91);
    configBase->writeToFile();

    Config configTest(*network);
    EXPECT_EQ(configTest.getCaptureMode(), true);
    EXPECT_EQ(configTest.getPassword(), "basepassword");
    EXPECT_EQ(configTest.getPhotosPer(), 90);
    EXPECT_EQ(configTest.getSeconds(), 91);
}

TEST_F(ConfigTests, Defaults) {
    remove("config.json");
    Config configTest(*network);
    EXPECT_EQ(configTest.getPassword(), "123");
}

TEST_F(ConfigTests, Saving) {
    configBase->setCaptureMode(false);
    configBase->setPassword("testpassword");
    configBase->setPhotosPer(50);
    configBase->setSeconds(51);
    configBase->writeToFile();

    Config configTest(*network);

    EXPECT_EQ(configTest.getCaptureMode(), false);
    EXPECT_EQ(configTest.getSeconds(), 51);
    EXPECT_EQ(configTest.getPhotosPer(), 50);
    EXPECT_EQ(configTest.getPassword(), "testpassword");

}






