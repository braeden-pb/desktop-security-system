//
// Created by Mitchell on 2026-03-06.
//

#include "Config.h"

Config::Config() {
    motionSensitivity = 0;
    captureMode = false;
    clipSeconds = 0;
    photosPer = 1;
    authorizedFaces.clear();
}

Config::~Config() {
    authorizedFaces.clear();
}

int Config::getSensitivity() {
    return motionSensitivity;
}

void Config::setSensitivity(int sensitivity) {
    motionSensitivity = sensitivity;
}

bool Config::getCaptureMode() {
    return captureMode;
}

void Config::setCaptureMode(bool mode) {
    captureMode = mode;
}

int Config::getSeconds() {
    return clipSeconds;
}

void Config::setSeconds(int seconds) {
    clipSeconds = seconds;
}

int Config::getPhotosPer() {
    return photosPer;
}

void Config::setPhotosPer(int photos) {
    photosPer = photosPer;
}

vector<string> Config::getAuthorizedFaces() {
    return authorizedFaces;
}

bool Config::removeAuthorizedFace(string face) {
    for (int i = 0; i < authorizedFaces.size(); i++) {
        if (authorizedFaces[i] == face) {
            authorizedFaces.erase(authorizedFaces.begin() + i);
            return true;
        }
    }
    return false;
}

bool Config::addAuthorizedFace(string face) {
    for (int i = 0; i < authorizedFaces.size(); i++) {
        if (authorizedFaces[i] == face) {
            return false;
        }
    }
    authorizedFaces.push_back(face);
    return true;
}

bool Config::isAuthorizedFace(string face) {
    for (int i = 0; i < authorizedFaces.size(); i++) {
        if (authorizedFaces[i] == face) {
            return true;
        }
    }
    return false;
}

void Config::resetAuthorizedFaces() {
    authorizedFaces.clear();
}


