//
// Created by evan on 2026-03-08.
//

#ifndef GROUP55_CONFIG_PANEL_H
#define GROUP55_CONFIG_PANEL_H

#include <wx-3.2/wx/wx.h>
class UI;
class SecuritySystem;


class Config_Panel {
    public:
    Config_Panel(wxWindow* parent, SecuritySystem* system, UI* mainFrame);
    ~Config_Panel();
    SecuritySystem* m_system;
    UI* m_ui;


private:
};


#endif //GROUP55_CONFIG_PANEL_H