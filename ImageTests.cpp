#include <gtest/gtest.h>
#include "Image.h"

// Test if Image correctly handles its resolution
TEST(SecuritySystemTests, ImageResolutionCheck) {
    // Arrange
    Image img(2, "path/to/img.jpg", "12:01", "4K", 300);

    // Act & Assert
    EXPECT_EQ(img.getSize(), "4K");
}

// Test 2: Verify inherited data from File parent
TEST(SecuritySystemTests, ImageInheritanceCheck) {
    Image img(10, "test.png", "09:00", "1080p", 72);

    EXPECT_EQ(img.getID(), 10);
    EXPECT_EQ(img.getPath(), "test.png");
}

// Test 3: Verify DPI specific data
TEST(SecuritySystemTests, ImageDPICheck) {
    Image img(3, "photo.jpg", "14:20", "8K", 600);

    // Assuming you have a getDPI() method
    EXPECT_EQ(img.getDPI(), 600);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}