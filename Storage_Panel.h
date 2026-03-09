//
// Created by evan on 2026-03-08.
//

#ifndef GROUP55_STORAGE_PANEL_H
#define GROUP55_STORAGE_PANEL_H

#include <wx-3.2/wx/wx.h>
#include <list>

class UI;
class SecuritySystem;

class Storage_Panel : public wxPanel {
public:
    Storage_Panel(wxWindow* parent, SecuritySystem* system, UI* ui);
    ~Storage_Panel();

    private:
    SecuritySystem* m_system;
    UI* m_ui;
    std::list<std::string> imageList;
    wxBitmap loadThumbnail(const wxString& filepath);

};


#endif //GROUP55_STORAGE_PANEL_H