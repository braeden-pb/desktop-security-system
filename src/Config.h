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
#include "Network.h"
using namespace std;

class Config {
public:
    Config(Network &network);
    ~Config();
    bool getCaptureMode();
    void setCaptureMode(bool mode);
    int getSeconds();
    void setSeconds(int seconds);
    int getPhotosPer();
    void setPhotosPer(int photos);
    string getPassword();
    void setPassword(string newPassword);
    bool writeToFile();
    string getSound();
    void setSound(string alarmSound);
    int getMaxAlarmDuration();
    void setMaxAlarmDuration(int maxAlarmDuration);
    bool getAlarmOnMotion();
    void setAlarmOnMotion(bool alarmOnMotion);

private:


    bool captureMode;


    int clipSeconds;


    int photosPer;

    string password;



    bool readFromFile();

    Network &network;

    string alarmSound;

    int maxAlarmDuration;

    bool alarmOnMotion;
};


#endif //GROUP55_CONFIG_H