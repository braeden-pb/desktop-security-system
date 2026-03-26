//
// Created by braed on 3/26/2026.
//

#include "Camera.h"

Camera::Camera() : recording(false) {}

Camera::~Camera() {}

std::string Camera::capturePhoto() {
    auto start = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(start);
    std::string filename = "../saved_data/photo_" + std::to_string(t) + ".jpg";

    std::string cmd = "libcamera-still -o " + filename + " --nopreview -t 1";
    int result = system(cmd.c_str());
    if (result != 0) {
        std::cerr << "Camera capture failed" << std::endl;
        return "";
    }

    lastCapture = filename;
    return filename;
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

bool Camera::isRecording() {
    return recording;
}
