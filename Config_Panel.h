//
// Created by evan on 2026-03-08.
//

#ifndef GROUP55_CONFIG_PANEL_H
#define GROUP55_CONFIG_PANEL_H

#include <wx/wx.h>
#include <wx/filepicker.h>
class UI;
class SecuritySystem;


class Config_Panel : wxPanel {
    public:
    Config_Panel(wxWindow* parent, SecuritySystem* system, UI* mainFrame);
    ~Config_Panel();
    SecuritySystem* m_system;
    UI* m_ui;


private:
    wxStaticText* m_staticText1;
    wxScrolledWindow* m_scrolledWindow1;
    wxStaticText* m_staticText3;
    wxTextCtrl* m_textCtrl2;
    wxButton* m_button1;
    wxStaticText* m_staticText5;
    wxSlider* m_slider1;
    wxStaticText* m_staticText4;
    wxFilePickerCtrl* m_filePicker1;
    wxStaticText* m_staticText6;
    wxRadioBox* m_radioBox1;
    wxButton* m_button2;
};


#endif //GROUP55_CONFIG_PANEL_H