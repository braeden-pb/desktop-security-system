//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#ifndef GROUP55_VIDEO_H
#define GROUP55_VIDEO_H

#include "File.h"
#include <string>

class Video : public File {
private:
    std::string videoLength;

public:
    Video(int id, std::string filePath, std::string timeStamp, std::string resolution, std::string videoLength);
    ~Video();
    void play();
    void pause();
    std::string getDuration();
};

#endif //GROUP55_VIDEO_H