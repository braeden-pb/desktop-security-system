#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "Storage.h"
#include "Image.h"

namespace fs = std::filesystem;

class StorageTest : public ::testing::Test {
protected:
    Storage store;

    // Define filenames we will use for testing
    std::string file1 = "test_img1.jpg";
    std::string file2 = "test_img2.jpg";

    // SetUp runs BEFORE every single TEST_F
    void SetUp() override {
        // Create actual dummy files on the disk
        std::ofstream(file1) << "dummy data 1";
        std::ofstream(file2) << "dummy data 2";

        // Ensure the saved_data directory is clean for the test
        if (fs::exists("../saved_data")) {
            fs::remove_all("../saved_data");
        }
        fs::create_directory("../saved_data");
    }

    // TearDown runs AFTER every single TEST_F
    void TearDown() override {
        // Remove the original dummy files
        fs::remove(file1);
        fs::remove(file2);
        // Optional: remove the saved_data folder to keep your project clean
        //fs::remove_all("./saved_data");
    }
};

// 1. Test saveImage() - Now verifies physical file creation
TEST_F(StorageTest, SaveImageMethod) {
    Image img1(1, file2, "10:00", "4K", 300);
    std::string savedPath = store.saveImage(img1, file2);

    // Verify the path returned is correct and the file physically exists there
    EXPECT_TRUE(fs::exists(savedPath));
    EXPECT_EQ(store.listImage().size(), 1);
}

// 2. Test deleteImage() - Now verifies physical file removal
TEST_F(StorageTest, DeleteImageSuccess) {
    Image img1(1, file1, "10:00", "4K", 300);
    std::string savedPath = store.saveImage(img1, file1);

    ASSERT_TRUE(fs::exists(savedPath)); // Pre-condition: file must exist

    bool result = store.deleteImage(1);

    EXPECT_TRUE(result);
    EXPECT_EQ(store.listImage().size(), 0);
    EXPECT_FALSE(fs::exists(savedPath)); // The file should be GONE from disk
}

// 3. Test listImage() - Verifies memory tracking with physical saving
TEST_F(StorageTest, ListImageMethod) {
    Image img1(1, file1, "10:00", "4K", 300);
    Image img2(2, file2, "10:05", "1080p", 72);

    store.saveImage(img1, file1);
    store.saveImage(img2, file2);

    EXPECT_EQ(store.listImage().size(), 2);
}

// 4. Test hasTimeStamp() - Logic remains the same, but data is now "real"
TEST_F(StorageTest, HasTimeStampMethod) {
    Image img2(2, file2, "10:05", "1080p", 72);
    store.saveImage(img2, file2);

    EXPECT_TRUE(store.hasTimeStamp(2));
    EXPECT_FALSE(store.hasTimeStamp(55));
}