//
// Created by Mitchell on 2026-03-06.
//

#ifndef GROUP55_CONFIG_H
#define GROUP55_CONFIG_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Config {
public:
    Config();
    ~Config();
    int getSensitivity();
    void setSensitivity(int sensitivity);
    bool getCaptureMode();
    void setCaptureMode(bool mode);
    int getSeconds();
    void setSeconds(int seconds);
    int getPhotosPer();
    void setPhotosPer(int photos);
    vector<string> getAuthorizedFaces();
    bool removeAuthorizedFace(string face);
    bool addAuthorizedFace(string face);
    bool isAuthorizedFace(string face);
    void resetAuthorizedFaces();
    bool writeToFile();

private:
    /**
     * Stores the sensitivity level for the motion detection as an integer value.
     */
    int motionSensitivity;

    /**
     * Stores the capture mode for the security system as a boolean value.
     */
    bool captureMode;

    /**
     * Stores the number of seconds per clip recorded as an integer value.
     */
    int clipSeconds;

    /**
     * Stores the number of photos taken per camera trigger as an integer value.
     */
    int photosPer;

    /**
     * Stores a list of strings representing all the authorized faces for the security system.
     */
    vector<string> authorizedFaces;
    bool readFromFile();
};


#endif //GROUP55_CONFIG_H