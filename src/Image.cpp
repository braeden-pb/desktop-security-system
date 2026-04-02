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
Image::Image(std::string filePath, std::string timeStamp)
    : File(filePath, timeStamp) {
}

//! Default destructor.
Image::~Image() {}


