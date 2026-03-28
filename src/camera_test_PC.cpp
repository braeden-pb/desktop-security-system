//
// Created by evan on 2026-03-27.
//

// pc_test.cpp
#include "Network.h"
#include "Camera.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>



int main() {
    Network network;

    if (!network.connect("192.168.2.174", 5000)) {
        std::cerr << "Failed to connect to Pi." << std::endl;
        return 1;
    }
    std::cout << "Connected!" << std::endl;

    Camera camera(network);
    network.startReceiving();

    // Shared frame buffer between network thread and main thread
    std::vector<uint8_t> pendingFrame;
    std::string pendingPhoto;
    std::mutex frameMutex;
    std::mutex photoMutex;

    // Network thread — just stores the latest frame
    camera.onFrame([&](const std::vector<uint8_t>& jpeg) {
        std::lock_guard<std::mutex> lock(frameMutex);
        pendingFrame = jpeg;
    });

    // Network thread — just stores the latest photo path
    camera.onPhoto([&](const std::string& path) {
        std::lock_guard<std::mutex> lock(photoMutex);
        pendingPhoto = path;
        std::cout << "Photo saved to: " << path << std::endl;
    });

    camera.startStream();

    std::cout << "Controls: [p] photo  [q] quit" << std::endl;

    while (true) {
        // Grab the latest frame if one arrived
        std::vector<uint8_t> frameToShow;
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            if (!pendingFrame.empty())
                frameToShow = std::move(pendingFrame);
        }

        // Decode and display on main thread
        if (!frameToShow.empty()) {
            cv::Mat frame = cv::imdecode(frameToShow, cv::IMREAD_COLOR);
            if (!frame.empty())
                cv::imshow("Pi Stream", frame);
        }

        // Show photo if one arrived
        std::string photoToShow;
        {
            std::lock_guard<std::mutex> lock(photoMutex);
            if (!pendingPhoto.empty())
                photoToShow = std::move(pendingPhoto);
        }
        if (!photoToShow.empty()) {
            cv::Mat img = cv::imread(photoToShow);
            if (!img.empty())
                cv::imshow("Last Photo", img);
        }

        // Process key on main thread
        char key = cv::waitKey(30);
        if (key == 'q') break;
        if (key == 'p') camera.capturePhoto();
    }

    camera.stopStream();
    network.stopReceiving();
    network.disconnect();
    return 0;
}