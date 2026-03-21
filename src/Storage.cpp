/**
 * @file Storage.cpp
 * @brief Implementation of the Storage class for image filesystem management.
 * @author Braeden Patierno-Barker
 * @date 3/6/2026
 */

#include "Storage.h"
#include <iostream>
#include <chrono>


/**
 * @brief Construct a new Storage object.
 * * Initializes the storage directory if it doesn't exist and synchronizes the
 * internal imageList with the files found on disk.
 * * @throws std::filesystem::filesystem_error if the directory iterator fails.
 */
namespace fs = std::filesystem;

Storage::Storage(std::string storagePath) : storagePath(storagePath) {
    if (!fs::exists(storagePath)) {
        fs::create_directory(storagePath);
    }

    try {
        int idCounter = 1;
        for (const auto& entry : fs::directory_iterator(storagePath)) {
            if (entry.is_regular_file()) {
                std::string path = entry.path().string();

                std::filesystem::file_time_type ftime = std::filesystem::last_write_time(path);
                auto sctime = std::chrono::file_clock::to_sys(ftime);
                auto sys_tp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(sctime);
                auto cftime = std::chrono::system_clock::to_time_t(sys_tp);
                std::string time_str = std::format("{:%Y-%m-%d %H:%M:%S}", sctime);
                imageList.push_back(Image(idCounter, path, time_str, "1080p", 0));
                idCounter++;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

Storage::~Storage() {}

/**
* @brief Saves an image to the local storage directory.
* * @param image The Image object metadata to be stored in the internal list.
* @param sourcePath The current path of the image on the disk.
* @return std::string The destination path where the image was saved, or empty string on failure.
*/
std::string Storage::saveImage(Image image, std::string sourcePath) {
    try {
        // Construct destination path
        std::string dest = storagePath + sourcePath;

        // Perform the physical file copy, replacing any existing file with the same name
        std::filesystem::copy(sourcePath, dest, std::filesystem::copy_options::overwrite_existing);

        // Update the object's path to the new internal location and track it
        image.setPath(dest);
        imageList.push_back(image);

        return dest;
    }
    catch (const std::exception& e) {
        std::cerr << "Save Error: " << e.what() << std::endl;
        return "";
    }
}

/**
 * @brief Deletes an image from the filesystem and the internal tracking list.
 * * @param imageID The unique identifier of the image to remove.
 * @return true If the image was successfully found and deleted.
 * @return false If the ID does not exist or a filesystem error occurred.
 */
bool Storage::deleteImage(int imageID) {
    try {
        // Iterates through all images
        for (auto it = imageList.begin(); it != imageList.end(); ++it) {
            if (it->getID() == imageID) {
                // Checks to see if the filepath exists
                if (std::filesystem::exists(it->getPath())) {
                    std::filesystem::remove(it->getPath());
                }

                imageList.erase(it);
                // Returns after deleting the image
                return true;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Delete Error: " << e.what() << std::endl;
    }
    return false;
}

/**
 * @brief Retrieves the current list of tracked images.
 * @return std::list<Image> A copy of the image metadata list.
 */
std::list<Image> Storage::listImage() {
    return imageList;
}

/**
 * @brief Checks if an image exists based on its ID.
 * @note Despite the name, this currently checks for existence, not just timestamp presence.
 * @param imageID The ID to search for.
 * @return true if the image is found in imageList.
 */
bool Storage::hasTimeStamp(int imageID) const {
    for (auto img : imageList) {
        if (img.getID() == imageID) {
            return true;
        }
    }

    return false;
}
