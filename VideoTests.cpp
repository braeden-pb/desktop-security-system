#include <gtest/gtest.h>
#include "Video.h"

// Test suite for the Video class methods
class VideoTest : public ::testing::Test {
protected:
    // Sample data for a security clip
    int id = 5;
    std::string path = "driveway_cam.mp4";
    std::string time = "2026-03-08 14:00";
    std::string res = "1080p";
    std::string duration = "00:45";
};

// 1. Test getDuration() method
TEST_F(VideoTest, GetDurationMethod) {
    Video v(id, path, time, res, duration, false, nullptr);
    EXPECT_EQ(v.getDuration(), "00:45");
}

// 2. Test initial playback state
TEST_F(VideoTest, InitialStateIsPaused) {
    // Note: The constructor forces isPlaying to false regardless of the passed bool
    Video v(id, path, time, res, duration, true, nullptr);
    
    // We expect it to be false because your constructor sets: isPlaying(false)
    // If you add a getIsPlaying() method, you would test it like this:
    // EXPECT_FALSE(v.getIsPlaying());
}

// 3. Test play() method with nullptr (Safety Check)
TEST_F(VideoTest, PlayWithNullPlayer) {
    Video v(id, path, time, res, duration, false, nullptr);
    
    // This should not crash because of your 'if (player)' check
    v.play();
    
    // Since player is null, isPlaying should still be false
    // EXPECT_FALSE(v.getIsPlaying());
}

// 4. Test pause() method with nullptr (Safety Check)
TEST_F(VideoTest, PauseWithNullPlayer) {
    Video v(id, path, time, res, duration, false, nullptr);
    
    // This should not crash because of your 'if (player)' check
    v.pause();
}

// 5. Test inheritance of File methods through Video
TEST_F(VideoTest, InheritedFileMethods) {
    Video v(id, path, time, res, duration, false, nullptr);
    
    EXPECT_EQ(v.getID(), 5);
    EXPECT_EQ(v.getPath(), "driveway_cam.mp4");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}