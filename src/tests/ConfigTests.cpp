//
// Created by Mitchell on 2026-03-10.
//

#include <gtest/gtest.h>
#include "../Config.h"
#include "../Network.h"
#include <fstream>

class ConfigTests : public ::testing::Test {
protected:
    void SetUp() override {
        network = new Network();
        configBase = new Config(*network);
    }
    void TearDown() override {
        delete configBase;
        delete network;
    }
    Network* network;
    Config* configBase;
};

TEST_F(ConfigTests, ReadFile) {
    remove("config.json");
    configBase->setCaptureMode(true);
    configBase->setPassword("basepassword");
    configBase->setPhotosPer(90);
    configBase->setSeconds(91);
    configBase->setSensitivity(92);
    configBase->addAuthorizedFace("face1");
    configBase->addAuthorizedFace("face2");
    configBase->addAuthorizedFace("face3");
    configBase->writeToFile();

    Config configTest(*network);
    EXPECT_EQ(configTest.getCaptureMode(), true);
    EXPECT_EQ(configTest.getPassword(), "basepassword");
    EXPECT_EQ(configTest.getPhotosPer(), 90);
    EXPECT_EQ(configTest.getSeconds(), 91);
    EXPECT_EQ(configTest.getSensitivity(), 92);
    EXPECT_EQ(configTest.isAuthorizedFace("face1"), true);
    EXPECT_EQ(configTest.isAuthorizedFace("face2"), true);
    EXPECT_EQ(configTest.isAuthorizedFace("face3"), true);
    EXPECT_EQ(configTest.isAuthorizedFace("face4"), false);
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
    configBase->setSensitivity(52);
    configBase->addAuthorizedFace("face1");
    configBase->addAuthorizedFace("face2");
    configBase->writeToFile();

    Config configTest(*network);
    EXPECT_EQ(configTest.getSensitivity(), 52);
    EXPECT_EQ(configTest.getCaptureMode(), false);
    EXPECT_EQ(configTest.getSeconds(), 51);
    EXPECT_EQ(configTest.getPhotosPer(), 50);
    EXPECT_EQ(configTest.getPassword(), "testpassword");
    EXPECT_EQ(configTest.isAuthorizedFace("face1"), true);
    EXPECT_EQ(configTest.isAuthorizedFace("face2"), true);
}

TEST_F(ConfigTests, RemoveFace) {
    remove("config.json");
    Config configTest(*network);
    configTest.addAuthorizedFace("face");
    configTest.removeAuthorizedFace("face");
    EXPECT_EQ(configTest.isAuthorizedFace("face"), false);
}

TEST_F(ConfigTests, RemoveFaceNotExist) {
    remove("config.json");
    Config configTest(*network);
    bool success = configTest.removeAuthorizedFace("face");
    EXPECT_EQ(success, false);
}

TEST_F(ConfigTests, AddDoubleFace) {
    remove("config.json");
    Config configTest(*network);
    configTest.addAuthorizedFace("face");
    bool success = configTest.addAuthorizedFace("face");
    EXPECT_EQ(success, false);
}
