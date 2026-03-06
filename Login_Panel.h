//
// Created by evan on 2026-03-06.
//

#ifndef GROUP55_LOGIN_PANEL_H
#define GROUP55_LOGIN_PANEL_H

#include <wx-3.2/wx/wx.h>

class UI;
class SecuritySystem;

class Login_Panel : public wxPanel {
public:
    Login_Panel(wxWindow* parent, SecuritySystem* system, UI* mainFrame);
    ~Login_Panel();

private:
    SecuritySystem* m_system;
    UI* m_ui;
    wxTextCtrl *pinInput;
    void onLogin(wxCommandEvent& event);
};


#endif //GROUP55_LOGIN_PANEL_H