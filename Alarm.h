//
// Created by rushd on 3/8/26.
//

#ifndef GROUP55_ALARM_H
#define GROUP55_ALARM_H

#include <string>

class Alarm {
private:
    bool isActive;
    int volume;
    std::string soundType;
    std::string lastActivatedAt;

public:
    Alarm();
    Alarm(int volume, const std::string& soundType);

    void activate();
    void activate(int durationSec);
    void deactivate();
    void testAlarm();
    bool getStatus() const;
};

#endif //GROUP55_ALARM_H