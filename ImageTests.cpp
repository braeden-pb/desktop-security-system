/**
* @file ImageTest.cpp
 * @brief Acceptance tests for the Image class.
 * @details Validates image-specific metadata and inheritance, supporting the "View gallery" user story.
 * @author Braeden Patierno-Barker
 * @date 3/9/2026
 */

#include <gtest/gtest.h>
#include "Image.h"

/**
 * @test Acceptance Test for "View Gallery": Verifies image resolution/size.
 * @details Confirms that image metadata accurately reflects the resolution.
 */
TEST(SecuritySystemTests, US_ViewGallery_ResolutionCheck) {
    // Arrange: Create image with specific resolution
    Image img(2, "path/to/img.jpg", "12:01", "4K", 300);

    // Act & Assert: Verify AC 45: View gallery metadata
    EXPECT_EQ(img.getSize(), "4K");
}

/**
 * @test Verifies that Image correctly inherits core metadata from the File class.
 * @details Ensures ID and Path consistency across the class hierarchy.
 */
TEST(SecuritySystemTests, ImageInheritanceCheck) {
    // Arrange: Initialize with base class parameters
    Image img(10, "test.png", "09:00", "1080p", 72);

    // Act & Assert: Confirm inheritance of ID and Path
    EXPECT_EQ(img.getID(), 10);
    EXPECT_EQ(img.getPath(), "test.png");
}

/**
 * @test Verifies image-specific DPI (Dots Per Inch) storage.
 * @details Ensures high-fidelity metadata is preserved for gallery viewing.
 */
TEST(SecuritySystemTests, ImageDPICheck) {
    // Arrange: Create image with high-density DPI
    Image img(3, "photo.jpg", "14:20", "8K", 600);

    // Act & Assert: Assuming the getDPI() implementation exists
    EXPECT_EQ(img.getDPI(), 600);
}

/**
 * @brief Main entry point for the Image test suite.
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}