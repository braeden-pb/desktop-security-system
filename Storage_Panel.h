//
// Created by evan on 2026-03-08.
//

#ifndef GROUP55_STORAGE_PANEL_H
#define GROUP55_STORAGE_PANEL_H

#include <wx/wx.h>
#include <list>

class UI;
class SecuritySystem;

class Storage_Panel : public wxPanel {
public:
    Storage_Panel(wxWindow* parent, SecuritySystem* system, UI* ui);
    ~Storage_Panel();
    void loadImages();

    private:
    SecuritySystem* m_system;
    UI* m_ui;
    wxStaticText* errorLabel;
    wxFlexGridSizer* gridSizer;
    wxScrolledWindow* galleryPanel;
    wxBoxSizer* outerSizer;
    std::list<std::pair<int, std::string>> imageList;
    wxBitmap loadThumbnail(const wxString& filepath);
    void OnGalleryResize(wxSizeEvent& event);
    void OpenFullImage(const wxString& path);
    void onBackHome(wxCommandEvent& event);

};


#endif //GROUP55_STORAGE_PANEL_H