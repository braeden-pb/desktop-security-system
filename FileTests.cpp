#include <gtest/gtest.h>
#include "File.h"

// Test the File class methods based on your implementation
class FileTest : public ::testing::Test {
protected:
    // Sample data for testing
    int testID = 500;
    std::string testPath = "/home/braeden/projects/group55/data.txt";
    std::string testTime = "2026-03-08 15:45";
    std::string testRes = "1920x1080";
};

// 1. Test the getID() method
TEST_F(FileTest, GetIDMethod) {
    File f(testID, testPath, testTime, testRes);
    EXPECT_EQ(f.getID(), 500);
}

// 2. Test the getPath() method
TEST_F(FileTest, GetPathMethod) {
    File f(testID, testPath, testTime, testRes);
    EXPECT_EQ(f.getPath(), "/home/braeden/projects/group55/data.txt");
}

// 3. Test the getTimeStamp() method
TEST_F(FileTest, GetTimeStampMethod) {
    File f(testID, testPath, testTime, testRes);
    EXPECT_EQ(f.getTimeStamp(), "2026-03-08 15:45");
}

// 4. Test the getResolution() method
TEST_F(FileTest, GetResolutionMethod) {
    File f(testID, testPath, testTime, testRes);
    EXPECT_EQ(f.getResolution(), "1920x1080");
}

// 5. Boundary Test: Verify handling of empty or zero values
TEST_F(FileTest, BoundaryValueHandling) {
    File f(0, "", "", "");
    EXPECT_EQ(f.getID(), 0);
    EXPECT_EQ(f.getPath(), "");
    EXPECT_EQ(f.getResolution(), "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}