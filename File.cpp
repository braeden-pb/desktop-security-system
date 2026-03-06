//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#include "File.h"

File::File(int id, std::string filePath, std::string timeStamp, std::string resolution)
    : id(id), filePath(filePath), timeStamp(timeStamp), resolution(resolution) {
}

File::~File() {}

int File::getID() {
    return id;
}

std::string File::getPath() {
    return filePath;
}

std::string File::getTimeStamp() {
    return timeStamp;
}

std::string File::getResolution() {
    return resolution;
}