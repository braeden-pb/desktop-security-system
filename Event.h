//
// Created by Michael Pachowski on 2026-03-06.
//

#ifndef GROUP55_EVENT_H
#define GROUP55_EVENT_H

#include <string>
#include "Device.h"
#include "Image.h"
#include <vector>

class Event {
private:
    std::string timeStamp;
    Device device;
    std::vector<Image> images;
public:
    Event(std::string time, Device device);
    std::string getTime();
    std::vector<Image> getImages();
    void deleteEvent();
};


#endif //GROUP55_EVENT_H