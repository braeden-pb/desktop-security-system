//
// Created by evan on 2026-03-21.
//

#ifndef GROUP55_PROTOCOL_H
#define GROUP55_PROTOCOL_H


#include <cstdint>

enum class System : uint8_t {
    Camera = 0x01,
    Motion = 0x02,
};

enum class Command : uint8_t {
    TakePhoto       = 0x01,
    StartClip       = 0x02,
    StopClip        = 0x03,
    Frame           = 0x04,

    Enable          = 0x10,
    Disable         = 0x11,
    MotionDetected  = 0x12,
    SetSensitivity  = 0x13,
};

struct PacketHeader {
    System   system;
    Command  command;
    uint32_t payloadSize;
};


#endif //GROUP55_PROTOCOL_H