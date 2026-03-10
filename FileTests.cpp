/**
 * @file FileTest.cpp
 * @brief Unit and Acceptance tests for the File base class.
 * @details Validates core metadata storage which supports the "Timestamp and log events" user story.
 * @author Braeden Patierno-Barker
 * @date 3/9/2026
 */

#include <gtest/gtest.h>
#include "File.h"

/**
 * @class FileTest
 * @brief Test fixture for the File class metadata verification.
 */
class FileTest : public ::testing::Test {
protected:
    //! Sample data representing a logged security event
    int testID = 500;
    std::string testPath = "/home/braeden/projects/group55/data.txt";
    std::string testTime = "2026-03-08 15:45";
    std::string testRes = "1920x1080";
};

/**
 * @test Verifies getID() correctly retrieves the unique identifier.
 */
TEST_F(FileTest, GetIDMethod) {
    File f(testID, testPath, testTime, testRes);
    EXPECT_EQ(f.getID(), 500);
}

/**
 * @test Verifies getPath() correctly retrieves the file location.
 */
TEST_F(FileTest, GetPathMethod) {
    File f(testID, testPath, testTime, testRes);
    EXPECT_EQ(f.getPath(), "/home/braeden/projects/group55/data.txt");
}

/**
 * @test Acceptance Test for "Timestamp and log events": Verifies timestamp retrieval.
 * @details Confirms that the system accurately stores the event time[cite: 36, 38].
 */
TEST_F(FileTest, US_Timestamp_GetMethod) {
    File f(testID, testPath, testTime, testRes);
    // Verifies that a motion detection event or capture is logged with a timestamp [cite: 40, 41]
    EXPECT_EQ(f.getTimeStamp(), "2026-03-08 15:45");
}

/**
 * @test Verifies getResolution() correctly retrieves media dimensions.
 */
TEST_F(FileTest, GetResolutionMethod) {
    File f(testID, testPath, testTime, testRes);
    EXPECT_EQ(f.getResolution(), "1920x1080");
}

/**
 * @test Boundary Test: Verify handling of empty or zero values.
 * @details Ensures the system doesn't crash on null metadata inputs.
 */
TEST_F(FileTest, BoundaryValueHandling) {
    File f(0, "", "", "");
    EXPECT_EQ(f.getID(), 0);
    EXPECT_EQ(f.getPath(), "");
    EXPECT_EQ(f.getResolution(), "");
}

/**
 * @brief Main entry point for GoogleTest runner.
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}