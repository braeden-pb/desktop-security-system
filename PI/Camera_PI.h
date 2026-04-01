//
// Created by evan on 2026-03-25.
//

#ifndef GROUP55_CAMERA_PI_H
#define GROUP55_CAMERA_PI_H

#include <string>
#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
#include <libcamera/libcamera/libcamera.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <fstream>
#include <sys/mman.h>
#include "NetworkServer.h"

using namespace libcamera;

struct Media {
    std::string filePath;
    std::string type;
};

class Camera_PI {
public:
    Camera_PI(NetworkServer &server);
    ~Camera_PI();

    std::string capturePhoto();
    void startRecording();
    void stopRecording();
    bool isRecording();
    void startStreaming();
    void stopStreaming();
    std::atomic<bool> streaming{false};
    std::thread streamThread;
private:
    bool recording;
    std::shared_ptr<CameraManager>      cm;
    std::shared_ptr<Camera>             camera;
    std::unique_ptr<CameraConfiguration> config;
    FrameBufferAllocator               *allocator = nullptr;
    std::vector<std::unique_ptr<Request>> requests;
    std::unique_ptr<std::ofstream> videoFile;

    NetworkServer &server;

    int initCamera();
    std::chrono::system_clock::time_point lastCaptureAt;
    std::string devicePath;
};

#endif