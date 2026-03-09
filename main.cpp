//
// Created by evan on 2026-02-19.
//

#include <wx/wx.h>
#include "UI.h"
#include "SecuritySystem.h"

class MyApp : public wxApp {
public:
    bool OnInit() override {
        SecuritySystem* system = new SecuritySystem();
        UI* mainUi = new UI(system);
        mainUi->Show(true);
        mainUi->Maximize(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);