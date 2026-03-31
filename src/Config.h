//
// Created by Mitchell on 2026-03-06.
//

#ifndef GROUP55_CONFIG_H
#define GROUP55_CONFIG_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

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
    string getPassword();
    void setPassword(string newPassword);
    vector<string> getAuthorizedFaces();
    bool removeAuthorizedFace(string face);
    bool addAuthorizedFace(string face);
    bool isAuthorizedFace(string face);
    void resetAuthorizedFaces();
    bool writeToFile();
    string getSound();
    void setSound(string alarmSound);
    int getMaxAlarmDuration();
    void setMaxAlarmDuration(int maxAlarmDuration);
    bool getAlarmOnMotion();
    void setAlarmOnMotion(bool alarmOnMotion);

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
     * Stores the password for the system.
     */
    string password;

    /**
     * Stores a list of strings representing all the authorized faces for the security system.
     */
    vector<string> authorizedFaces;
    bool readFromFile();

    string alarmSound;

    int maxAlarmDuration;

    bool alarmOnMotion;
};


#endif //GROUP55_CONFIG_H