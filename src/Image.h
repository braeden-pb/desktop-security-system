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
    Image(std::string filePath, std::string timeStamp);
    ~Image();
    std::string getSize();

};

#endif //GROUP55_IMAGE_H