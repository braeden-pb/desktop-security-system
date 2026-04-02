//
// Created by evan on 2026-03-08.
//

#ifndef GROUP55_CONFIG_PANEL_H
#define GROUP55_CONFIG_PANEL_H

#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include "Config.h"
#include "SecuritySystem.h"
#include "UI.h"
class UI;
class SecuritySystem;


class Config_Panel : public wxPanel {
    public:
    Config_Panel(wxWindow* parent, SecuritySystem* system, UI* mainFrame);
    ~Config_Panel();
    SecuritySystem* m_system;
    UI* m_ui;
    bool changesMade;
    bool changesSaved;


private:
    wxStaticText* title;
    wxScrolledWindow* innerScrollWindow;
    wxButton* PasswordBtn;
    wxButton* ClipLenBtn;
    wxButton* motionBtn;
    wxButton* alarmConfigBtn;
    wxButton* SaveBtn;
    wxButton* homeBtn;
    void onChangePass(wxCommandEvent& event);
    void onChangeClipLen(wxCommandEvent& event);
    void onChangeMotion(wxCommandEvent& event);
    void onChangeAlarm(wxCommandEvent& event);
    void onSave(wxCommandEvent& event);
    void onHomeButton(wxCommandEvent& event);
};


#endif //GROUP55_CONFIG_PANEL_H
