//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

#ifndef GROUP55_FILE_H
#define GROUP55_FILE_H

#include <string>

class File {
protected:
    int id;
    std::string filePath;
    std::string timeStamp;
    std::string resolution;

public:
    File(int id, std::string filePath, std::string timeStamp, std::string resolution);
    virtual ~File();
    int getID();
    std::string getPath() const;
    std::string getTimeStamp();
    std::string getResolution();
    void setPath(std::string newPath);

};

#endif //GROUP55_FILE_H