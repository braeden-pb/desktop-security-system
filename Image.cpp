//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#include "Image.h"

Image::Image(int id, std::string filePath, std::string timeStamp, std::string resolution, int dpi)
    : File(id, filePath, timeStamp, resolution), dpi(dpi) {
}

Image::~Image() {}

std::string Image::getSize() {
    return resolution;
}