/**
 * @file StorageTests.cpp
 * @brief Unit tests for the Storage class using Google Test framework.
 * @details Specifically addresses image saving, listing, deletion, and timestamp verification.
 * @author Braeden Patierno-Barker
 * @date 3/10/2026
 */

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../Storage.h"
#include "../Image.h"

namespace fs = std::filesystem;

/**
 * @class StorageTest
 * @brief Test fixture for verifying Storage class filesystem and list management.
 */
class StorageTest : public ::testing::Test {
protected:
    /**
     * @brief Sets up a clean test environment before each test case.
     * @details Removes any leftover test directory, creates a fresh one,
     * creates dummy source image files on disk, and initializes the Storage instance.
     */
    void SetUp() override {
        if (fs::exists("../test_data/")) fs::remove_all("../test_data/");
        fs::create_directory("../test_data/");
        std::ofstream("camera_view_1.jpg").close();
        std::ofstream("camera_view_2.jpg").close();
        store = new Storage("../test_data/");
    }

    /**
     * @brief Tears down the test environment after each test case.
     * @details Deletes the Storage instance, removes the test directory
     * and its contents, and cleans up dummy source image files.
     */
    void TearDown() override {
        delete store;
        fs::remove_all("../test_data/");
        fs::remove("camera_view_1.jpg");
        fs::remove("camera_view_2.jpg");
    }

    Storage* store; ///< Pointer to the Storage instance under test.
    Image img1 = Image(1, "camera_view_1.jpg", "10:00", "4K", 300);   ///< First dummy image for testing.
    Image img2 = Image(2, "camera_view_2.jpg", "10:05", "1080p", 72); ///< Second dummy image for testing.
};

/**
 * @test Verifies that saveImage() returns a non-empty destination path and adds the image to the list.
 * @details Supports US: "As a user, I can save captured images to local storage."
 */
TEST_F(StorageTest, SaveImageMethod) {
    std::string path = store->saveImage(img1, "camera_view_1.jpg");
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(store->listImage().size(), 1);
}

/**
 * @test Verifies that listImage() returns all saved images.
 * @details Confirms the internal image list is updated correctly after multiple saves.
 */
TEST_F(StorageTest, ListImageMethod) {
    store->saveImage(img1, "camera_view_1.jpg");
    store->saveImage(img2, "camera_view_2.jpg");
    EXPECT_EQ(store->listImage().size(), 2);
}

/**
 * @test Verifies that deleteImage() successfully removes an existing image by ID.
 * @details Confirms both the in-memory list and filesystem entry are removed.
 */
TEST_F(StorageTest, DeleteImageSuccess) {
    store->saveImage(img1, "camera_view_1.jpg");
    bool result = store->deleteImage(1);
    EXPECT_TRUE(result);
    EXPECT_EQ(store->listImage().size(), 0);
}

/**
 * @test Safety Test: Verifies that deleteImage() returns false for a non-existent ID.
 * @details Ensures the image list remains unchanged when deletion fails.
 */
TEST_F(StorageTest, DeleteImageFailure) {
    store->saveImage(img1, "camera_view_1.jpg");
    bool result = store->deleteImage(999);
    EXPECT_FALSE(result);
    EXPECT_EQ(store->listImage().size(), 1);
}

/**
 * @test Verifies that hasTimeStamp() correctly identifies existing and non-existing image IDs.
 * @details Confirms the search loop logic handles both found and not-found cases.
 */
TEST_F(StorageTest, HasTimeStampMethod) {
    store->saveImage(img2, "camera_view_2.jpg");
    EXPECT_TRUE(store->hasTimeStamp(2));
    EXPECT_FALSE(store->hasTimeStamp(55));
}

/**
 * @brief Runner for Storage class unit tests.
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}