//
// Created by evan on 2026-03-25.
//

#include "Camera_PI.h"
#include <iostream>
#include <stdexcept>

Camera_PI::Camera_PI()
    : resolution("1920x1080"),
      fps(30),
      isRecording(false),
      devicePath("/dev/video0")
{
}

Media Camera_PI::capturePhoto() {

}

void Camera_PI::startRecording() {

}

Media Camera_PI::stopRecording() {

}

void Camera_PI::setResolution(std::string resolution) {

}
