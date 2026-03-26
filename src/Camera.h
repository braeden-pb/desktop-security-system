//
// Created by braed on 3/26/2026.
//

#ifndef GROUP55_CAMERA_H
#define GROUP55_CAMERA_H

#include <string>
#include <thread>
#include <iostream>
#include <chrono>
#include <ctime>

class Camera {
    public:
        Camera();
        ~Camera();
        std::string capturePhoto();
        void startRecording();
        void stopRecording();
        bool isRecording();

    private:
        bool recording;
        std::string lastCapture;
        std::string devicePath;
        std::thread recordThread;
        void recordingThread();
};


#endif //GROUP55_CAMERA_H