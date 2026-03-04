//
// Created by evan on 2026-02-24.
//

#ifndef GROUP55_HOME_PANEL_H
#define GROUP55_HOME_PANEL_H

#include <wx-3.2/wx/wx.h>


class UI;
class SecuritySystem;

class Home_Panel : public wxPanel {
public:
    Home_Panel(wxWindow* parent, SecuritySystem* system, UI* mainFrame);
    ~Home_Panel();

private:
    SecuritySystem* m_system;
    UI* m_ui;
    wxPanel *mainPanel;
    void onConfigButtonPressed(wxCommandEvent& event);
    void onArmButtonPressed(wxCommandEvent& event);
    void onDisarmButtonPressed(wxCommandEvent& event);
};


#endif //GROUP55_HOME_PANEL_H