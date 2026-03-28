/**
* @file File.cpp
 * @brief Implementation of the File class representing image metadata.
 * @author Braeden Patierno-Barker
 * @date 3/6/2026
 */

#include "File.h"

/**
 * @brief Construct a new File object with specific metadata.
 * * @param id Unique identifier for the file.
 * @param filePath The initial path to the file on the disk.
 * @param timeStamp The creation or last modified string.
 * @param resolution The image dimensions (e.g., "1080p").
 */
File::File(int id, std::string filePath, std::string timeStamp, std::string resolution)
    : id(id), filePath(filePath), timeStamp(timeStamp), resolution(resolution) {
}

//! Default destructor.
File::~File() {}

/**
 * @brief Gets the unique ID of the file.
 * @return int The file's identifier.
 */
int File::getID() const {
    return id;
}

/**
 * @brief Gets the current filesystem path of the file.
 * @return std::string The full path string.
 */
std::string File::getPath() const {
    return filePath;
}

/**
 * @brief Gets the timestamp string associated with the file.
 * @return std::string The formatted date/time string.
 */
std::string File::getTimeStamp() const{
    return timeStamp;
}

/**
 * @brief Gets the resolution metadata of the image.
 * @return std::string The resolution string.
 */
std::string File::getResolution() const{
    return resolution;
}

/**
 * @brief Updates the stored path of the file.
 * @note This only updates the metadata; it does not move the file on disk.
 * @param newPath The new destination path.
 */
void File::setPath(std::string newPath) {
    this->filePath = newPath;
}
