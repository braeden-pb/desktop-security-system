//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#ifndef GROUP55_STORAGE_H
#define GROUP55_STORAGE_H

#include <list>
#include <string>

class Storage {
public:
    Storage();
    ~Storage();
    std::string saveImage(Image image);
    bool deleteImage(Image imageID);
    list<Image> listImage();
    bool hasTimeStamp(String imageID);
};

#endif //GROUP55_STORAGE_H