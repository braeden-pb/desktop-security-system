//
// Created by Michael Pachowski on 2026-03-06.
//

#ifndef GROUP55_ALERT_H
#define GROUP55_ALERT_H

#include <string>
#include "Event.h"
#include "Observer.h"
#include "SecuritySystem.h"
#include <iostream>

class SecuritySystem;

class Alert : public Observer {
private:
    Event event;
    //std::string email;
    SecuritySystem& system;

public:
    Alert(Event event,SecuritySystem& system);
    void motionDetected();
    void sendAlert();
    Event getEvent();
    void discard();
    void update(const std::string& eventMsg) override;
    virtual ~Alert() = default;
};

#endif //GROUP55_ALERT_H
