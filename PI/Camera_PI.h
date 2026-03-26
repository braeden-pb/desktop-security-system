//
// Created by evan on 2026-03-25.
//

#ifndef GROUP55_CAMERA_PI_H
#define GROUP55_CAMERA_PI_H

#include <string>
#include <chrono>

struct Media {
    std::string filePath;
    std::string type;
};

class Camera_PI {
public:
    Camera_PI();
    ~Camera_PI();

    Media capturePhoto();
    void startRecording();
    Media stopRecording();
    void setResolution(const std::string& res);

private:
    std::string resolution;
    int fps;
    bool isRecording;
    std::chrono::system_clock::time_point lastCaptureAt;
    std::string devicePath;
};

#endif