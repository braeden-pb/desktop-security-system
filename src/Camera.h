//
// Created by braed on 3/26/2026.
//

#ifndef GROUP55_CAMERA_H
#define GROUP55_CAMERA_H

#include <vector>
#include <functional>
#include <string>
#include <cstdint>
#include <mutex>
#include <thread>
#include <chrono>
#include "Network.h"
#include "../Shared/Protocol.h"
#include <iostream>

class Network;

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>
#include "../Shared/Protocol.h"
#include "Network.h"

class Camera {
public:
    Camera(Network &network);
    ~Camera();

    std::string capturePhoto();
    void startRecording();
    void stopRecording();
    bool isRecording() const;

    void onFrame(std::function<void(const std::vector<uint8_t>&)> callback);
    void onPhoto(std::function<void(const std::string&)> callback);
    std::vector<uint8_t> getLatestFrame();
    bool hasNewFrame() const;
    std::string lastPhotoPath() const;

private:
    void handlePacket(Command cmd, const std::vector<uint8_t> &payload);

    Network &network;
    bool recording;
    std::string lastPhoto;
    std::thread recordThread;

    std::function<void(const std::vector<uint8_t>&)> frameCallback;
    std::function<void(const std::string&)> photoCallback;

    mutable std::mutex frameMutex;
    std::vector<uint8_t> latestFrame;
    bool newFrameAvailable = false;
};

#endif //GROUP55_CAMERA_H