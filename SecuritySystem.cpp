//
// Created by evan on 2026-02-19.
//

#include "SecuritySystem.h"
#include "UI.h"

SecuritySystem::SecuritySystem() {
    mainUi = std::make_unique<UI>(this);

}

UI *SecuritySystem::getUI() {
    return mainUi.get();
}
