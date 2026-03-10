//
// Created by Michael Pachowski on 2026-03-06.
//

#ifndef GROUP55_ALERT_H
#define GROUP55_ALERT_H

#include <string>
#include "Event.h"

class SecuritySystem;

class IAlertCallback {
public:
    virtual void onAlert(const std::string& email, const std::string& time) = 0;
    virtual ~IAlertCallback() = default;
};

class Alert {
private:
    IAlertCallback* uiCallback = nullptr;
    Event& event;
    std::string email;
    SecuritySystem& system;

public:
    Alert(Event& event, std::string email, SecuritySystem& system);

    void setUICallback(IAlertCallback* cb) {
        uiCallback = cb;
    }

    void sendAlert();
    Event getEvent();
    void discard();
};

#endif //GROUP55_ALERT_H