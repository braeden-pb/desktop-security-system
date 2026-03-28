#include "Camera.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <filesystem>

Camera::Camera(Network &network) : recording(false), network(network) {
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

// ── Incoming packets from Pi ──────────────────────────────────────────────────

void Camera::handlePacket(Command cmd, const std::vector<uint8_t> &payload) {
    switch (cmd) {
        case Command::Frame: {
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
            if (payload.empty()) break;

            std::filesystem::create_directories("../saved_data");

            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::ostringstream oss;
            oss << "../saved_data/photo_" << t << ".jpg";
            lastPhoto = oss.str();

            std::ofstream file(lastPhoto, std::ios::binary);
            file.write(reinterpret_cast<const char*>(payload.data()), payload.size());
            file.close();

            std::cout << "Photo saved to: " << lastPhoto << std::endl;

            if (photoCallback)
                photoCallback(lastPhoto);
            break;
        }
        default:
            break;
    }
}

// ── Stream access for UI ──────────────────────────────────────────────────────

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