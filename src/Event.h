#ifndef GROUP55_EVENT_H
#define GROUP55_EVENT_H

#include <string>

class Event {
private:
    std::string timeStamp;
    std::string type;

public:
    Event(std::string time);
    Event(std::string time, std::string type);
    std::string getTime() const;
    std::string getType() const;

};

#endif
