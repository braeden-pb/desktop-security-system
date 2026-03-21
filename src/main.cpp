//
// Created by evan on 2026-02-19.
//

#include <wx/wx.h>
#include "UI.h"
#include "SecuritySystem.h"

class MyApp : public wxApp {
public:
    bool OnInit() override {
        auto* system = new SecuritySystem();
        UI* mainUI = system->getUI();
        wxInitAllImageHandlers();
        mainUI->Show(true);
        mainUI->Maximize(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);