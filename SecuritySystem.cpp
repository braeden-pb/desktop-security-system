//
// Created by evan on 2026-02-19.
//

#include "SecuritySystem.h"

#include "Alarm.h"
#include "UI.h"
#include "Storage.h"

SecuritySystem::SecuritySystem() {
    mainStorage = std::make_unique<Storage>();
    mainUi = std::make_unique<UI>(this);
    systemStatus = Status::disarmed;
    alarm = std::make_unique<Alarm>();


}

SecuritySystem::SecuritySystem(bool headless) {
    mainStorage = std::make_unique<Storage>();
    if (!headless) mainStorage = std::make_unique<Storage>();
    systemStatus = Status::disarmed;
}

SecuritySystem::~SecuritySystem() {}

UI *SecuritySystem::getUI() const{
    return mainUi.get();
}

void SecuritySystem::setStatus(Status status) {
    systemStatus = status;
}

void SecuritySystem::soundAlarm() {
    alarm->activate();
}

void SecuritySystem::turnOffAlarm() {
    alarm->deactivate();
}

bool SecuritySystem::getIsAlarmActive() const {
    return alarm->getStatus();
}

bool SecuritySystem::validatePIN(const std::string& pin) const{
    return pin=="1234";
}


bool SecuritySystem::isArmed() const {
    return systemStatus == Status::armed;
}

void SecuritySystem::arm() {
    setStatus(Status::armed);
}

void SecuritySystem::disarm() {
    setStatus(Status::disarmed);
}

Storage *SecuritySystem::getStorage() const{
    return mainStorage.get();
}

std::list<std::tuple<int,std::string,std::string>> SecuritySystem::getAllImages() {
    std::list<std::tuple<int,std::string,std::string>> paths;
    // Get the list of objects from your storage class
    auto images = mainStorage->listImage();

        for (const auto& img : images) {
            paths.emplace_back(img.getID(), img.getTimeStamp(), img.getPath());
        }
    return paths;
}


