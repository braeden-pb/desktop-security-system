//
// Created by evan on 2026-02-19.
//
#ifndef GROUP55_SECURITYSYSTEM_H
#define GROUP55_SECURITYSYSTEM_H
#include <string>
#include <memory>

#include "Storage.h"

enum class Status : int {
    armed,
    disarmed,
    Error
};

class UI;
class Storage;

class SecuritySystem
{
public:
    SecuritySystem();
    ~SecuritySystem();
    Status getStatus();
    void arm();
    void disarm();
    //getConfig
    std::list<std::pair<int, std::string>> getAllImagePaths();
    Storage* getStorage();
    //getDeviceList
    void soundAlarm();
    UI* getUI();
    Status systemStatus;


private:
    //Config object
    //Devices object list
    std::unique_ptr<Storage> mainStorage;
    std::unique_ptr<UI> mainUi;
    void setStatus(Status status);
    void initializeSystem();
    void setStatus(Status status);

};


#endif //GROUP55_SECURITYSYSTEM_H