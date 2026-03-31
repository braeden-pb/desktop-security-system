#include "Event.h"

Event::Event(std::string time) {
    timeStamp = time;
}

Event::Event(std::string time, std::string type) {
    timeStamp = time;
    this->type = type;
}

std::string Event::getTime() const{
    return timeStamp;
}

std::string Event::getType() const {
    return type;
}
