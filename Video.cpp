//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#include "Video.h"

Video::Video(int id, std::string filePath, std::string timeStamp, std::string resolution, std::string videoLength, bool isPlaying, wxMediaCtrl* player)
    : File(id, filePath, timeStamp, resolution), videoLength(videoLength), isPlaying(false), player(player) {
}

Video::~Video() {}

void Video::play() {
    if (player && !isPlaying) {
        if (player->Load(filePath)) {
            player->Play();
            isPlaying = true;
        }
    }
}

void Video::pause() {
    if (player && isPlaying) {
        player->Pause();
        isPlaying = false;
    }
}

std::string Video::getDuration() {
    return videoLength;
}