/**
* @file Image.cpp
 * @brief Implementation of the Image class, specializing the File base class.
 * @author Braeden Patierno-Barker
 * @date 3/6/2026
 */

#include "Image.h"

/**
 * @brief Construct a new Image object.
 * * This constructor initializes the base File metadata and adds image-specific
 * properties like DPI.
 * * @param id Unique identifier.
 * @param filePath Path to the image file.
 * @param timeStamp Metadata timestamp.
 * @param resolution The resolution (e.g., "1920x1080").
 * @param dpi Dots Per Inch, representing print/display density.
 */
Image::Image(int id, std::string filePath, std::string timeStamp, std::string resolution, int dpi)
    : File(id, filePath, timeStamp, resolution), dpi(dpi) {
}

//! Default destructor.
Image::~Image() {}

/**
 * @brief Retrieves the image resolution.
 * @note This is an alias for the base class resolution metadata.
 * @return std::string The resolution string.
 */
std::string Image::getSize() {
    return resolution;
}

/**
 * @brief Gets the Dots Per Inch (DPI) of the image.
 * @return int The DPI value.
 */
int Image::getDPI() {
    return dpi;
}