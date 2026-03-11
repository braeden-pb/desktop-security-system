#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../Storage.h"
#include "../Image.h"

namespace fs = std::filesystem;

class StorageTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (fs::exists("../saved_data/")) fs::remove_all("../saved_data/");
        fs::create_directory("../saved_data/");
        std::ofstream("camera_view_1.jpg").close();
        std::ofstream("camera_view_2.jpg").close();
        store = new Storage();
    }

    void TearDown() override {
        delete store;
        if (fs::exists("../saved_data/")) //fs::remove_all("../saved_data/");
        fs::remove("camera_view_1.jpg");
        fs::remove("camera_view_2.jpg");
    }

    Storage* store;
    Image img1 = Image(1, "camera_view_1.jpg", "10:00", "4K", 300);
    Image img2 = Image(2, "camera_view_2.jpg", "10:05", "1080p", 72);
};

TEST_F(StorageTest, SaveImageMethod) {
    std::string path = store->saveImage(img1, "camera_view_1.jpg");
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(store->listImage().size(), 1);
}

TEST_F(StorageTest, ListImageMethod) {
    store->saveImage(img1, "camera_view_1.jpg");
    store->saveImage(img2, "camera_view_2.jpg");
    EXPECT_EQ(store->listImage().size(), 2);
}

TEST_F(StorageTest, DeleteImageSuccess) {
    store->saveImage(img1, "camera_view_1.jpg");
    bool result = store->deleteImage(1);
    EXPECT_TRUE(result);
    EXPECT_EQ(store->listImage().size(), 0);
}

TEST_F(StorageTest, DeleteImageFailure) {
    store->saveImage(img1, "camera_view_1.jpg");
    bool result = store->deleteImage(999);
    EXPECT_FALSE(result);
    EXPECT_EQ(store->listImage().size(), 1);
}

TEST_F(StorageTest, HasTimeStampMethod) {
    store->saveImage(img2, "camera_view_2.jpg");
    EXPECT_TRUE(store->hasTimeStamp(2));
    EXPECT_FALSE(store->hasTimeStamp(55));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}