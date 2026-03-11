//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#ifndef GROUP55_STORAGE_H
#define GROUP55_STORAGE_H

#include <list>
#include <string>
#include <filesystem>
#include <chrono>
#include "Image.h"

class Storage {

private:
    std::list<Image> imageList;
    std::string storagePath;

public:
    Storage();
    ~Storage();
    std::string saveImage(Image image, std::string sourcePath = "");    bool deleteImage(int imageID);
    std::list<Image> listImage();
    bool hasTimeStamp(int imageID);
};

#endif //GROUP55_STORAGE_H