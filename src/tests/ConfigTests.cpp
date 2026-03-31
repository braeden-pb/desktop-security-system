//
// Created by Mitchell on 2026-03-10.
//

#include <gtest/gtest.h>
#include "../Config.h"
#include <fstream>

/**
 * @class ConfigTests
 * @brief Tests basic use cases for the Config object.
 */
class ConfigTests : public ::testing::Test {
protected:
    Config configBase;
};

// 1. Test saving config file variables to file and then creating new config with these values
TEST_F(ConfigTests, ReadFile) {
    configBase.setCaptureMode(true);
    configBase.setPassword("basepassword");
    configBase.setPhotosPer(90);
    configBase.setSeconds(91);
    configBase.setSensitivity(92);
    configBase.addAuthorizedFace("face1");
    configBase.addAuthorizedFace("face2");
    configBase.addAuthorizedFace("face3");
    configBase.writeToFile();

    Config configTest;

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

// 2. Test creating new config with default values
TEST_F(ConfigTests, Defaults) {
    remove("config.txt");
    Config configTest;

    EXPECT_EQ(configTest.getPassword(), "123");
}

// 3. Test saving config values to file
TEST_F(ConfigTests, Saving) {
    configBase.setCaptureMode(false);
    configBase.setPassword("testpassword");
    configBase.setPhotosPer(50);
    configBase.setSeconds(51);
    configBase.setSensitivity(52);
    configBase.addAuthorizedFace("face1");
    configBase.addAuthorizedFace("face2");
    configBase.writeToFile();

    ifstream fileRead("config.txt");
    string inText;

    getline(fileRead, inText);

    EXPECT_EQ(inText, "52");
    getline(fileRead, inText);
    EXPECT_EQ(inText, "0");
    getline(fileRead, inText);
    EXPECT_EQ(inText, "51");
    getline(fileRead, inText);
    EXPECT_EQ(inText, "50");
    getline(fileRead, inText);
    EXPECT_EQ(inText, "testpassword");
    getline(fileRead, inText);
    EXPECT_EQ(inText, "face1");
    getline(fileRead, inText);
    EXPECT_EQ(inText, "face2");
}

// 4. Test removing of face
TEST_F(ConfigTests, RemoveFace) {
    remove("config.txt");
    Config configTest;
    configTest.addAuthorizedFace("face");
    configTest.removeAuthorizedFace("face");

    EXPECT_EQ(configTest.isAuthorizedFace("face"), false);
}

// 5. Test removing face that does not exist
TEST_F(ConfigTests, RemoveFaceNotExist) {
    remove("config.txt");
    Config configTest;
    bool success = configTest.removeAuthorizedFace("face");

    EXPECT_EQ(success, false);
}

// 6. Test adding duplicate faces
TEST_F(ConfigTests, AddDoubleFace) {
    remove("config.txt");
    Config configTest;
    configTest.addAuthorizedFace("face");
    bool success = configTest.addAuthorizedFace("face");

    EXPECT_EQ(success, false);
}
