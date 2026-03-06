//
// Created by Mitchell on 2026-03-06.
//

#ifndef GROUP55_CONFIG_H
#define GROUP55_CONFIG_H
#include <vector>
#include <string>

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

private:
    int motionSensitivity;
    bool captureMode;
    int clipSeconds;
    int photosPer;
    vector<string> authorizedFaces;
};


#endif //GROUP55_CONFIG_H