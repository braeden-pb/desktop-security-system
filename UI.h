//
// Created by evan on 2026-02-23.
//

#ifndef GROUP55_UI_H
#define GROUP55_UI_H

#include <wx-3.2/wx/wx.h>
#include <wx/simplebook.h>

class SecuritySystem;

class UI : public wxFrame {
    public:
    UI(SecuritySystem* system);
    ~UI();
    enum PageID {
        Home_ID = 0,
        Setting_ID = 1,
        Storage_ID = 2,
        Dashboard_ID = 3
    };
    void SwitchPage(PageID id);

    private:
    SecuritySystem* m_system;
    wxSimplebook* book;
    wxMenuBar *menuBar;



};


#endif //GROUP55_UI_H