//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#include "Storage.h"

Storage::Storage() {}

Storage::~Storage() {}

std::string Storage::saveImage(Image image) {
    imageList.push_back(image);
    return image.getPath();
}

bool Storage::deleteImage(int imageID) {
    for (auto it = imageList.begin(); it != imageList.end(); ++it) {
        if (it->getID() == imageID) {
            imageList.erase(it);
            return true;
        }
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