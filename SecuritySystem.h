//
// Created by evan on 2026-02-19.
//
#ifndef GROUP55_SECURITYSYSTEM_H
#define GROUP55_SECURITYSYSTEM_H
#include <string>
#include <memory>
enum class Status : int {
    armed,
    disarmed,
    Error
};

class UI;

class SecuritySystem
{
public:
    SecuritySystem();
    ~SecuritySystem();
    Status getStatus();
    void arm(const std::string& password);
    void disarm(const std::string& password);
    //getConfig
    //getStorage
    //getDeviceList
    void soundAlarm();
    UI* getUI();
    Status systemStatus;
    void setStatus(Status status);



private:
    //Config object
    //Devices object list
    //Storage Object
    std::unique_ptr<UI> mainUi;
    void initializeSystem();

};


#endif //GROUP55_SECURITYSYSTEM_H