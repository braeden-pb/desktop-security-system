/**
 * @file StorageTest.cpp
 * @brief Acceptance tests for User Stories related to Image Management.
 * @details Specifically addresses US #1 (Save Image) and US #4 (Delete Image).
 */

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "Storage.h"
#include "Image.h"

namespace fs = std::filesystem;

class StorageTest : public ::testing::Test {
protected:
    // Use a unique pointer so we can restart the Storage service for every test
    std::unique_ptr<Storage> store;

    std::string file1 = "test_img1.jpg";

    void SetUp() override {
        // 1. Create source dummy file
        std::ofstream(file1) << "dummy data";

        // 2. Clean the environment
        if (fs::exists("../saved_data")) {
            //fs::remove_all("../saved_data");
        }
        fs::create_directory("../saved_data");

        // 3. Initialize the storage object AFTER the directory is ready
        store = std::make_unique<Storage>();
    }

    void TearDown() override {
        if (fs::exists(file1)) fs::remove(file1);
    }
};

/**
 * @test Acceptance Test for User Story #1: "As a user, I want to save images."
 * @details Verifies acceptance criteria: File exists on disk and metadata is tracked.
 */
TEST_F(StorageTest, US1_SaveImageAcceptance) {
    Image img1(1, file1, "10:00", "4K", 300);
    std::string savedPath = store->saveImage(img1, file1);

    EXPECT_TRUE(fs::exists(savedPath)) << "AC 1.1: Physical file must be created.";
    EXPECT_EQ(store->listImage().size(), 1) << "AC 1.2: Metadata must be added to list.";
}

/**
 * @test Acceptance Test for User Story #4: "As a user, I want to delete images."
 */
TEST_F(StorageTest, US4_DeleteImageAcceptance) {
    Image img1(1, file1, "10:00", "4K", 300);
    std::string savedPath = store->saveImage(img1, file1);

    // Dynamically fetch the ID assigned by the store
    int id = store->listImage().back().getID();

    bool result = store->deleteImage(id);

    EXPECT_TRUE(result);
    EXPECT_FALSE(fs::exists(savedPath)) << "AC 4.1: File must be removed from disk.";
    EXPECT_EQ(store->listImage().size(), 0) << "AC 4.2: List must be empty.";
}