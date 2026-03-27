//
// Created by braed on 3/26/2026.
//

#include "Camera.h"

Camera::Camera(Network &network) : recording(false),network(network) {
    network.onPacket(System::Camera, [this](Command cmd, const std::vector<uint8_t> &payload) {
        handlePacket(cmd, payload);
    });
}

Camera::~Camera() {}

std::string Camera::capturePhoto() {
        PacketHeader header{};
        header.system      = System::Camera;
        header.command     = Command::TakePhoto;
        header.payloadSize = 0;
        network.send(header, {});
        return "";
    }




void Camera::startRecording() {
    if (recording) return;

    recording = true;
    recordThread = std::thread(&Camera::startRecording, this);
}

void Camera::recordingThread() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::string filename = "../saved_data/video_" + std::to_string(t) + ".mp4";

    std::string cmd = "libcamera-vid -o " + filename + " --nopreview -t 0";
    system(cmd.c_str());
    lastCapture = filename;
}

void Camera::stopRecording() {
    if (recording) return;

    recording = false;
    system("pkill libcamera-vid");
    if (recordThread.joinable()) recordThread.join();
}

bool Camera::isRecording() const {
    return recording;
}
