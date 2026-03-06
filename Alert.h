//
// Created by Michael Pachowski on 2026-03-06.
//

#ifndef GROUP55_ALERT_H
#define GROUP55_ALERT_H

#include <string>
#include "Event.h"

class SecuritySystem;

class Alert {
private:
    Event& event;
    std::string email;
    SecuritySystem& system;

public:
    Alert(Event& event, std::string email, SecuritySystem& system);

    void sendAlert();
    Event getEvent();
    void discard();
};


#endif //GROUP55_ALERT_H