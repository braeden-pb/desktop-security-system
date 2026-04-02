#include "Camera.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <filesystem>

#include "Storage.h"

Camera::Camera(Network &network,Storage &storage) : recording(false), network(network),storage(storage) {
    network.onPacket(System::Camera, [this](Command cmd, const std::vector<uint8_t> &payload) {
        handlePacket(cmd, payload);
    });
}

Camera::~Camera() {
    if (streaming) stopStream();
    if (recording) stopRecording();
}


std::string Camera::capturePhoto() {
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::TakePhoto;
    header.payloadSize = 0;
    network.send(header, {});
    return lastPhoto; // will be populated when Pi responds
}

void Camera::startStream() {
    if (streaming) return;
    std::cout << "Sending StartStream command..." << std::endl;
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::StartStream;
    header.payloadSize = 0;
    network.send(header, {});
    streaming = true;
}

void Camera::stopStream() {
    if (!streaming) return;
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::StopStream;
    header.payloadSize = 0;
    network.send(header, {});
    streaming = false;
}

bool Camera::isStreaming() const {
    return streaming;
}

void Camera::startRecording() {
    if (recording) return;
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::StartClip;
    header.payloadSize = 0;
    network.send(header, {});
    recording = true;
}

void Camera::stopRecording() {
    if (!recording) return;  // was inverted
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::StopClip;
    header.payloadSize = 0;
    network.send(header, {});
    recording = false;
}

bool Camera::isRecording() const {
    return recording;
}


void Camera::handlePacket(Command cmd, const std::vector<uint8_t> &payload) {
    //std::cout << "Packet received, command: " << static_cast<int>(cmd) << std::endl;
    switch (cmd) {
        case Command::Frame: {
            //std::cout << "Frame packet: " << payload.size() << " bytes" << std::endl;
            {
                std::lock_guard<std::mutex> lock(frameMutex);
                latestFrame       = payload;
                newFrameAvailable = true;
            }
            if (frameCallback)
                frameCallback(payload);
            break;
        }
        case Command::TakePhoto: {
            std::cout << "Photo packet: " << payload.size() << " bytes" << std::endl;
            if (payload.empty()) break;


            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::ostringstream oss;
            oss << "../saved_data/photo_" << t << ".jpg";
            lastPhoto = oss.str();



            std::ofstream file(lastPhoto, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Failed to open file for writing: " << lastPhoto << std::endl;
                break;
            }
            file.write(reinterpret_cast<const char*>(payload.data()), payload.size());
            file.close();

            std::cout << "Photo saved to: " << lastPhoto << std::endl;
            std::tm* tmInfo = std::localtime(&t);
            std::ostringstream timeOss;
            timeOss << std::put_time(tmInfo, "%b %d, %Y  %I:%M %p");
            storage.addImage(lastPhoto,timeOss.str());

            if (photoCallback)
                photoCallback(lastPhoto);
            break;
        }

        case Command::StartClip: {
            std::cout << "Video clip received: " << payload.size() << " bytes" << std::endl;
            if (payload.empty()) break;

            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::ostringstream oss;
            oss << "../saved_data/video_" << t << ".avi";
            std::string videoPath = oss.str();

            std::ofstream file(videoPath, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Failed to open file for writing: " << videoPath << std::endl;
                break;
            }
            file.write(reinterpret_cast<const char*>(payload.data()), payload.size());
            file.close();

            std::tm* tmInfo = std::localtime(&t);
            std::ostringstream timeOss;
            timeOss << std::put_time(tmInfo, "%b %d, %Y  %I:%M %p");
            storage.addImage(videoPath, timeOss.str());

            std::cout << "Video saved to: " << videoPath << std::endl;
            break;
        }
        default:
            std::cout << "Unhandled command: " << static_cast<int>(cmd) << std::endl;
            break;
    }
}


void Camera::onFrame(std::function<void(const std::vector<uint8_t>&)> callback) {
    frameCallback = callback;
}

void Camera::onPhoto(std::function<void(const std::string&)> callback) {
    photoCallback = callback;
}

std::vector<uint8_t> Camera::getLatestFrame() {
    std::lock_guard<std::mutex> lock(frameMutex);
    newFrameAvailable = false;
    return latestFrame;
}

bool Camera::hasNewFrame() const {
    std::lock_guard<std::mutex> lock(frameMutex);
    return newFrameAvailable;
}

std::string Camera::lastPhotoPath() const {
    return lastPhoto;
}
