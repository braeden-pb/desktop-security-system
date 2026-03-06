//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#include "Video.h"

Video::Video(int id, std::string filePath, std::string timeStamp, std::string resolution, std::string videoLength)
    : File(id, filePath, timeStamp, resolution), videoLength(videoLength) {
}

Video::~Video() {}

void Video::play() {

}

void Video::pause() {

}

std::string Video::getDuration() {
    return videoLength;
}