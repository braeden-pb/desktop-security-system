// #include <gtest/gtest.h>
// #include "Image.h"
//
// // Test if Image correctly handles its resolution
// TEST(SecuritySystemTests, ImageResolutionCheck) {
//     // Arrange
//     Image img(2, "path/to/img.jpg", "12:01", "4K", 300);
//
//     // Act & Assert
//     EXPECT_EQ(img.getSize(), "4K");
// }
//
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

#include <iostream>
#include <string>
#include "Image.h"

// Helper function to print test results
void runTest(const std::string& testName, bool condition) {
    if (condition) {
        std::cout << "[ PASSED ] " << testName << std::endl;
    } else {
        std::cerr << "[ FAILED ] " << testName << std::endl;
        exit(1); // Stop if a critical test fails
    }
}

int main() {
    std::cout << "Running Manual Media Hierarchy Tests..." << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    // --- TEST 1: Image Sibling Inheritance ---
    // Creating an Image with: ID: 2, Path: pic.jpg, Time: 12:01, Res: 4K, DPI: 300
    Image img(2, "path/to/img.jpg", "12:01", "4K", 300);

    // Verify Shared Parent Data (Inherited from File)
    runTest("Image ID Check", img.getID() == 2);
    runTest("Image Path Check", img.getPath() == "path/to/img.jpg");

    // Verify Unique Sibling Data
    runTest("Image Resolution Check (getSize)", img.getSize() == "4K");

    return 0;
}