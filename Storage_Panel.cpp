//
// Created by evan on 2026-03-08.
//

#include "Storage_Panel.h"

Storage_Panel::Storage_Panel(wxWindow *parent, SecuritySystem *system, UI *ui) :
wxPanel(parent, wxID_ANY), m_system(system), m_ui(ui) {

    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxScrolledWindow* galleryPanel = new wxScrolledWindow(this, wxID_ANY);

    imageList;

    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(0, 4, 10, 10);

    for (const auto& path : imageList) {
        wxStaticBitmap* thumb = new wxStaticBitmap(galleryPanel, wxID_ANY, loadThumbnail(path));

        // Bind a click event to open the full image
        // thumb->Bind(wxEVT_LEFT_DOWN, [path](wxMouseEvent& event) {
        //     OpenFullImage(path);
        // });

        gridSizer->Add(thumb, 0, wxALL, 5);
    }

    galleryPanel->SetSizer(gridSizer);
    galleryPanel->SetScrollRate(0, 10);

    mainSizer->Add(galleryPanel, 1, wxEXPAND);
    this->SetSizer(mainSizer);

}

wxBitmap Storage_Panel::loadThumbnail(const wxString& path) {
    wxImage img(path, wxBITMAP_TYPE_ANY);
    if (!img.IsOk()) return wxNullBitmap;

    int thumbW = 150;
    int thumbH = 150;

    double ratio = (double)img.GetWidth() / img.GetHeight();
    if (img.GetWidth() > img.GetHeight()) {
        thumbH = thumbW / ratio;
    } else {
        thumbW = thumbH * ratio;
    }

    return wxBitmap(img.Scale(thumbW, thumbH, wxIMAGE_QUALITY_HIGH));
}


Storage_Panel::~Storage_Panel() {

}
