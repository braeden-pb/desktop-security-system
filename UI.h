//
// Created by evan on 2026-02-23.
//

#ifndef GROUP55_UI_H
#define GROUP55_UI_H

#include <wx-3.2/wx/wx.h>
#include <wx/simplebook.h>
#include "Home_Panel.h"
#include "Login_Panel.h"

class SecuritySystem;

class UI : public wxFrame {
    public:
    UI(SecuritySystem* system);
    ~UI();
    enum PageID {
        Login_ID = 0,
        Home_ID = 1,
        Storage_ID = 2,
        Setting_ID = 3
    };
    void SwitchPage(PageID id);
    void onExit(wxCommandEvent& event);
    PageID GetPageID();
    Login_Panel* getLoginPanel();


    private:
    SecuritySystem* m_system;
    wxSimplebook* book;
    wxMenuBar *menuBar;
    Login_Panel* loginPage;



};


#endif //GROUP55_UI_H