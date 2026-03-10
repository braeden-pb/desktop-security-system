//
// Created by Mitchell on 2026-03-06.
//

#include "Config.h"
using namespace std;

Config::Config() {
    if (this->readFromFile() == false) { //Attempt to read values from file
        //If the file cannot be read from, specify this and then set default values for all variables.
        std::cout << "Failed to read from file.";
        motionSensitivity = 0;
        captureMode = false;
        clipSeconds = 0;
        photosPer = 1;
        authorizedFaces.clear();
    }
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
    photosPer = photos;
}

vector<string> Config::getAuthorizedFaces() {
    return authorizedFaces;
}

bool Config::removeAuthorizedFace(string face) {
    for (int i = 0; i < authorizedFaces.size(); i++) {
        if (authorizedFaces[i] == face) { //If the face is found, remove this value.
            authorizedFaces.erase(authorizedFaces.begin() + i);
            return true;
        }
    }
    return false; //Return false if face never found.
}

bool Config::addAuthorizedFace(string face) {
    for (int i = 0; i < authorizedFaces.size(); i++) {
        if (authorizedFaces[i] == face) { //If the face already exists in the list, return false
            return false;
        }
    }
    //If it does not exist, add it to the list and return true.
    authorizedFaces.push_back(face);
    return true;
}

bool Config::isAuthorizedFace(string face) {
    for (int i = 0; i < authorizedFaces.size(); i++) {
        if (authorizedFaces[i] == face) { //If the face is found, return true
            return true;
        }
    }
    return false; //Return false if nothing was found
}

void Config::resetAuthorizedFaces() {
    authorizedFaces.clear();
}

bool Config::writeToFile() {
    try {
        ofstream configFile("config.txt");

        configFile << motionSensitivity << endl;
        configFile << captureMode << endl;
        configFile << clipSeconds << endl;
        configFile << photosPer << endl;

        for (int i = 0; i < authorizedFaces.size(); i++) { //Add each entry from the list to the file in a different line.
            configFile << authorizedFaces[i] << endl;
        }

        configFile.close();
    } catch (...) { //If any error is caught, return false.
        return false;
    }

    return true; //Return true if no errors
}

bool Config::readFromFile() {
    try {
        string inText;
        ifstream configFile("config.txt");

        getline(configFile, inText);
        this.motionSensitivity = stoi(inText);

        getline(configFile, inText);
        if (inText == "1")
            captureMode = true;
        else
            captureMode = false;

        getline(configFile, inText);
        this.clipSeconds = stoi(inText);

        getline(configFile, inText);
        this.photosPer = stoi(inText);

        authorizedFaces.clear(); //Clear authorized faces list before refilling it
        while (getline (configFile, inText)) {
            this->addAuthorizedFace(inText);

        }
    } catch (...) { //If any error occurs, return false.
        return false;
    }

    return true; //Return true if no errors
}
