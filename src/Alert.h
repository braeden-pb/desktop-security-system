//
// Created by Michael Pachowski on 2026-03-06.
//

#ifndef GROUP55_ALERT_H
#define GROUP55_ALERT_H

#include <string>
#include "Observer.h"

class SecuritySystem;

class Alert : public Observer {
private:
    SecuritySystem& system;

public:
    Alert(SecuritySystem& system);
    void motionDetected();
    void sendAlert();
    void discard();
    void update(const std::string& eventMsg) override;
    virtual ~Alert() = default;
};

#endif //GROUP55_ALERT_H
