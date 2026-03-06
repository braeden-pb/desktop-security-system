//
// Created by Michael Pachowski on 2026-03-06.
//

#include "Event.h"

Event::Event(std::string time, Device dev) {
    timeStamp = time;
    device = dev;
}
    std::string Event::getTime() {
        return timeStamp;
}

    std::vector<Image> Event::getImages() {
    return images;
}

    void Event::deleteEvent() {
        images.clear();
}