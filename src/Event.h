#ifndef GROUP55_EVENT_H
#define GROUP55_EVENT_H

#include <string>

class Event {
private:
    std::string timeStamp;

public:
    Event(std::string time);
    std::string getTime() const;
};

#endif