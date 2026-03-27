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
#include "../Shared/Protocol.h"
#include <iostream>

class Network;

class Camera {
    public:
        Camera(Network &network);
        ~Camera();
        std::string capturePhoto();
        void startRecording();
        void stopRecording();
        bool isRecording() const;

        void onFrame(std::function<void(const std::vector<uint8_t>&)> callback);
        std::vector<uint8_t> getLatestFrame();
        bool hasNewFrame() const;

    private:
        bool recording;
        std::string lastCapture;
        std::string devicePath;
        std::thread recordThread;
        Network &network;
        void recordingThread();
        void handlePacket(Command cmd, const std::vector<uint8_t> &payload);
        std::function<void(const std::vector<uint8_t>&)> frameCallback;
        mutable std::mutex frameMutex;
        std::vector<uint8_t> latestFrame;
        bool newFrameAvailable = false;
        std::string lastPhoto;
};


#endif //GROUP55_CAMERA_H