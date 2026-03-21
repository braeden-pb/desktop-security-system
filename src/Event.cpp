#include "Event.h"

Event::Event(std::string time) {
    timeStamp = time;
}

std::string Event::getTime() const{
    return timeStamp;
}