//
// Created by evan on 2026-03-27.
//

// pc_test.cpp
#include "Network.h"
#include "Camera.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    Network network;

    if (!network.connect("192.168.2.174", 8080)) {  // replace with your Pi's IP
        std::cerr << "Failed to connect to Pi." << std::endl;
        return 1;
    }
    std::cout << "Connected!" << std::endl;

    Camera camera(network);
    network.startReceiving();

    // Display frames as they arrive
    camera.onFrame([](const std::vector<uint8_t>& jpeg) {
        // Decode JPEG bytes directly into an OpenCV image
        cv::Mat frame = cv::imdecode(jpeg, cv::IMREAD_COLOR);
        if (frame.empty()) return;
        cv::imshow("Pi Stream", frame);
        cv::waitKey(1);  // 1ms wait — keeps the window responsive
    });

    // Save photos when they arrive
    camera.onPhoto([](const std::string& path) {
        std::cout << "Photo saved to: " << path << std::endl;
        cv::Mat img = cv::imread(path);
        if (!img.empty()) {
            cv::imshow("Last Photo", img);
            cv::waitKey(1);
        }
    });

    // Start the stream
    camera.startStream();

    // Simple keyboard controls
    std::cout << "Controls: [p] photo  [r] record 5s  [q] quit" << std::endl;
    while (true) {
        char key = cv::waitKey(30);  // check for keypress every 30ms
        if (key == 'q') break;
        if (key == 'p') camera.capturePhoto();
        if (key == 'r') camera.recordForSeconds(5);
    }

    camera.stopStream();
    network.stopReceiving();
    network.disconnect();
    return 0;
}