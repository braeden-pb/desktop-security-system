#include "SecuritySystem.h"

#include "Alarm.h"
#include "UI.h"
#include "Storage.h"
#include "Alert.h"


SecuritySystem::SecuritySystem() {
    mainStorage = std::make_unique<Storage>();
    mainUi = std::make_unique<UI>(this);
    systemStatus = Status::disarmed;
    alarm = std::make_unique<Alarm>();
    addObserver(mainUi.get());
    addObserver(alarm.get());


}

SecuritySystem::SecuritySystem(bool headless) {
    mainStorage = std::make_unique<Storage>();
    if (!headless) mainStorage = std::make_unique<Storage>();
    systemStatus = Status::disarmed;
    alarm = std::make_unique<Alarm>();
    addObserver(alarm.get());
}

SecuritySystem::~SecuritySystem() {}

UI *SecuritySystem::getUI() const{
    return mainUi.get();
}

void SecuritySystem::setStatus(Status status) {
    systemStatus = status;
}

void SecuritySystem::soundAlarm() {
    notifyObservers("Alarm triggered");
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

void SecuritySystem::addObserver(Observer* o) {
    observers.push_back(o);
}

void SecuritySystem::notifyObservers(const std::string& event) {
    for (auto* o : observers) {
        o->update(event);
    }
}

void SecuritySystem::soundAlert() {
    notifyObservers("Motion detected");
}



