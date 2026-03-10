//
// Created by evan on 2026-02-19.
//

#include "SecuritySystem.h"
#include "UI.h"
#include "Storage.h"

SecuritySystem::SecuritySystem() {
    mainStorage = std::make_unique<Storage>();
    mainUi = std::make_unique<UI>(this);
    systemStatus = Status::armed;


}

UI *SecuritySystem::getUI() {
    return mainUi.get();
}

void SecuritySystem::setStatus(Status status) {
    systemStatus = status;
}

std::list<std::string> SecuritySystem::getAllImagePaths() {
    std::list<std::string> paths;
    // Get the list of objects from your storage class
    auto images = mainStorage->listImage();

        for (const auto& img : images) {
            paths.push_back(img.getPath());
        }
    return paths;
}


