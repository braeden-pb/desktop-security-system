//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#ifndef GROUP55_VIDEO_H
#define GROUP55_VIDEO_H

#include "File.h"
#include <string>
#include <wx/wx.h>
#include <wx/mediactrl.h>

class Video : public File {
private:
    std::string videoLength;
    bool isPlaying = false;
    wxMediaCtrl* player = nullptr;

public:
    Video(int id, std::string filePath, std::string timeStamp, std::string resolution, std::string videoLength, bool isPlaying, wxMediaCtrl* player);
    ~Video();
    void play();
    void pause();
    std::string getDuration();
};

#endif //GROUP55_VIDEO_H