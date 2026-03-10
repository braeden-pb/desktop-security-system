//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#include "Storage.h"
#include <iostream>

namespace fs = std::filesystem;

Storage::Storage() {
    storagePath = "../saved_data/";
    if (!fs::exists(storagePath)) {
        fs::create_directory(storagePath);
    }

    try {
        int idCounter = 1;
        for (const auto& entry : fs::directory_iterator(storagePath)) {
            if (entry.is_regular_file()) {
                std::string path = entry.path().string();

                std::filesystem::file_time_type ftime = std::filesystem::last_write_time(path);
                auto sctime = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
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

std::string Storage::saveImage(Image image, std::string sourcePath) {
    try {
        std::string dest = "../saved_data/" + sourcePath;

        std::filesystem::copy(sourcePath, dest, std::filesystem::copy_options::overwrite_existing);

        image.setPath(dest);
        imageList.push_back(image);

        return dest;
    }
    catch (const std::exception& e) {
        std::cerr << "Save Error: " << e.what() << std::endl;
        return "";
    }
}

bool Storage::deleteImage(int imageID) {
    try {
        for (auto it = imageList.begin(); it != imageList.end(); ++it) {
            if (it->getID() == imageID) {
                if (std::filesystem::exists(it->getPath())) {
                    std::filesystem::remove(it->getPath());
                }

                imageList.erase(it);
                return true;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Delete Error: " << e.what() << std::endl;
    }
    return false;
}

std::list<Image> Storage::listImage() {
    return imageList;
}

bool Storage::hasTimeStamp(int imageID) {
    for (auto img : imageList) {
        if (img.getID() == imageID) {
            return true;
        }
    }

    return false;
}