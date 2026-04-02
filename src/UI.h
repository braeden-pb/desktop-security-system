//
// Created by evan on 2026-02-23.
//

#ifndef GROUP55_UI_H
#define GROUP55_UI_H

#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/display.h>
#include "Home_Panel.h"
#include "Login_Panel.h"
#include "Storage_Panel.h"
#include "Config_Panel.h"
#include "Observer.h"


class SecuritySystem;

class UI : public wxFrame,public Observer{
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
    PageID GetPageID() const;
    Login_Panel* getLoginPanel();
    Storage_Panel* getStoragePanel() const;
    void update(const std::string& event) override;
    void showAlert(const std::string& message);


    private:
    SecuritySystem* m_system;
    wxSimplebook* book;
    wxMenuBar *menuBar;
    Login_Panel* loginPage;
    Storage_Panel* storagePage;
    Home_Panel* homePage;
    Config_Panel* configPage;
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);



};


#endif //GROUP55_UI_H
