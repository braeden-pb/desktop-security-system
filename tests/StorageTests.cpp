#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../Storage.h"
#include "../Image.h"

namespace fs = std::filesystem;

class StorageTest : public ::testing::Test {
protected:
    Storage store;
    // Helper images for testing
    Image img1 = Image(1, "camera_view_1.jpg", "10:00", "4K", 300);
    Image img2 = Image(2, "camera_view_2.jpg", "10:05", "1080p", 72);
};

// 1. Test saveImage() - Verifies the path is returned and item is added
TEST_F(StorageTest, SaveImageMethod) {
    std::string path = store.saveImage(img1);

    EXPECT_EQ(path, "camera_view_1.jpg");
    EXPECT_EQ(store.listImage().size(), 1);
}

// 2. Test listImage() - Verifies we can retrieve the full list
TEST_F(StorageTest, ListImageMethod) {
    store.saveImage(img1);
    store.saveImage(img2);

    std::list<Image> results = store.listImage();
    EXPECT_EQ(results.size(), 2);
}

// 3. Test deleteImage() - Successful deletion via iterator logic
TEST_F(StorageTest, DeleteImageSuccess) {
    store.saveImage(img1);

    bool result = store.deleteImage(1);
    EXPECT_TRUE(result);
    EXPECT_EQ(store.listImage().size(), 0);
}

// 4. Test deleteImage() - Failure case when ID doesn't exist
TEST_F(StorageTest, DeleteImageFailure) {
    store.saveImage(img1);

    bool result = store.deleteImage(999); // Non-existent ID
    EXPECT_FALSE(result);
    EXPECT_EQ(store.listImage().size(), 1);
}

// 5. Test hasTimeStamp() - Verifies the search loop logic
TEST_F(StorageTest, HasTimeStampMethod) {
    store.saveImage(img2);

    EXPECT_TRUE(store.hasTimeStamp(2));
    EXPECT_FALSE(store.hasTimeStamp(55));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}