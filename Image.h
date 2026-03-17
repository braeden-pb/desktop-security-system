//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#ifndef GROUP55_IMAGE_H
#define GROUP55_IMAGE_H

#include "File.h"
#include <string>

class Image : public File {
private:
    int dpi;

public:
    Image(int id, std::string filePath, std::string timeStamp, std::string resolution, int dpi);
    ~Image();
    std::string getSize();
    int getDPI() const;
};

#endif //GROUP55_IMAGE_H