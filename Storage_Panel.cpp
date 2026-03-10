//
// Created by evan on 2026-03-08.
//

#include "Storage_Panel.h"

#include "SecuritySystem.h"
#include "UI.h"


Storage_Panel::Storage_Panel(wxWindow *parent, SecuritySystem *system, UI *ui) :
wxPanel(parent, wxID_ANY), m_system(system), m_ui(ui) {

    outerSizer = new wxBoxSizer(wxVERTICAL);

    auto* topBarSizer = new wxBoxSizer(wxHORIZONTAL);

    auto* titleText = new wxStaticText(this, wxID_ANY, "Storage");
    wxFont titleFont(28, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    wxSize trueSize = titleText->GetBestSize();
    titleText->SetMinSize(wxSize(trueSize.x + 10, trueSize.y));

    auto homeBtn = new wxButton(this,wxID_ANY,"Back Home");

    topBarSizer->Add(homeBtn,0,wxLEFT,10);
    topBarSizer->AddStretchSpacer(1);
    topBarSizer->Add(titleText, 0, wxALIGN_CENTER_VERTICAL);
    topBarSizer->AddStretchSpacer(1);

    outerSizer->Add(topBarSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 20);

    galleryPanel = new wxScrolledWindow(this, wxID_ANY);


    imageList = system->getAllImagePaths();

    gridSizer = new wxFlexGridSizer(0, 4, 15, 15);
    for (int i = 0; i < 4; i++) {
        gridSizer->AddGrowableCol(i, 1);
    }

    wxBoxSizer* horizontalCenteringSizer = new wxBoxSizer(wxHORIZONTAL);
    horizontalCenteringSizer->AddStretchSpacer(1);
    horizontalCenteringSizer->Add(gridSizer, 0, wxALIGN_CENTER);
    horizontalCenteringSizer->AddStretchSpacer(1);

    galleryPanel->SetSizer(horizontalCenteringSizer);

    galleryPanel->SetScrollRate(0, 10);

    galleryPanel->Bind(wxEVT_SIZE, &Storage_Panel::OnGalleryResize, this);

    outerSizer->Add(galleryPanel, 1, wxEXPAND | wxALL, 20);
    this->SetSizer(outerSizer);

    errorLabel = new wxStaticText(this, wxID_ANY, "");
    homeBtn->Bind(wxEVT_BUTTON,&Storage_Panel::onBackHome,this);

}

wxBitmap Storage_Panel::loadThumbnail(const wxString& path) {

    wxLogNull noLog;
    wxImage img(path, wxBITMAP_TYPE_ANY);
    if (!img.IsOk()) return wxNullBitmap;

    const int maxSize = 250;
    int w = img.GetWidth();
    int h = img.GetHeight();

    if (w > h) {
        h = (int)(maxSize * (double)h / w);
        w = maxSize;
    } else {
        w = (int)(maxSize * (double)w / h);
        h = maxSize;
    }

    return wxBitmap(img.Scale(w, h, wxIMAGE_QUALITY_HIGH));
}

void Storage_Panel::loadImages() {
    imageList.clear();
    gridSizer->Clear(true);
    galleryPanel->FitInside();
    galleryPanel->Layout();


    imageList = m_system->getAllImagePaths();

    if (imageList.empty()) {
        errorLabel->SetLabel("No Images or Videos");
        outerSizer->AddStretchSpacer(1);
        outerSizer->Add(errorLabel, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 50);
        return;
    }
    for (const auto& [id,path] : imageList) {
        wxBitmap thumbnail = loadThumbnail(path);
        if (!thumbnail.IsOk()) {
            continue;
        }
        // Container panel for thumb + overlay
        wxPanel* itemPanel = new wxPanel(galleryPanel, wxID_ANY);
        itemPanel->SetMinSize(wxSize(250, 250));

        wxStaticBitmap* thumb = new wxStaticBitmap(itemPanel, wxID_ANY, thumbnail);
        thumb->SetPosition(wxPoint(0, 0));
        thumb->SetSize(wxSize(250, 250));
        thumb->Bind(wxEVT_LEFT_DOWN, [this, path](wxMouseEvent&) {
            OpenFullImage(path);
        });

        // Small delete button in top-right corner
        wxButton* deleteBtn = new wxButton(itemPanel, wxID_ANY, "x",
            wxPoint(210, 5), wxSize(35, 35));
        deleteBtn->SetBackgroundColour(wxColour(180, 30, 30));
        deleteBtn->SetForegroundColour(*wxWHITE);
        deleteBtn->SetWindowStyleFlag(wxBORDER_NONE);

        // Hover effect
        deleteBtn->Bind(wxEVT_ENTER_WINDOW, [deleteBtn](wxMouseEvent& e) {
            deleteBtn->SetBackgroundColour(wxColour(220, 50, 50));
            deleteBtn->Refresh();
            e.Skip();
        });
        deleteBtn->Bind(wxEVT_LEAVE_WINDOW, [deleteBtn](wxMouseEvent& e) {
            deleteBtn->SetBackgroundColour(wxColour(180, 30, 30));
            deleteBtn->Refresh();
            e.Skip();
        });

        deleteBtn->Bind(wxEVT_BUTTON, [this, id,path, itemPanel](wxCommandEvent&) {
            int result = wxMessageBox("Are you sure you want to delete this file","Confirm?",wxYES_NO);
        if (result==wxYES) {
            m_system->getStorage()->deleteImage(id);
            itemPanel->Destroy();
            galleryPanel->FitInside();
            galleryPanel->Layout();
        }

        });

        gridSizer->Add(itemPanel, 1, wxEXPAND | wxALL, 5);
    }
    galleryPanel->FitInside();
}

void Storage_Panel::onBackHome(wxCommandEvent &event) {
    m_ui->SwitchPage(UI::Home_ID);
}

void Storage_Panel::OnGalleryResize(wxSizeEvent& event) {
    int panelWidth = event.GetSize().GetWidth();
    int thumbSize = 265; // 250px + 15px gap
    int cols = std::max(1, panelWidth / thumbSize);

    gridSizer->SetCols(cols);
    galleryPanel->Layout();
    event.Skip();
}


void Storage_Panel::OpenFullImage(const wxString& path) {
    wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Image Viewer", wxDefaultPosition,
                                  wxDefaultSize, wxDEFAULT_FRAME_STYLE);

    frame->ShowFullScreen(true);
    frame->SetBackgroundColour(*wxBLACK);

    wxImage img(path, wxBITMAP_TYPE_ANY);
    if (!img.IsOk()) {
        frame->Destroy();
        return;
    }

    wxPanel* panel = new wxPanel(frame, wxID_ANY, wxDefaultPosition,
                              wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS);
    panel->SetBackgroundColour(*wxBLACK);

    wxPanel* topBar = new wxPanel(panel, wxID_ANY);
    topBar->SetBackgroundColour(wxColour(30, 30, 30));

    wxBoxSizer* topBarSizer = new wxBoxSizer(wxHORIZONTAL);
    topBarSizer->AddStretchSpacer(1);

    wxButton* closeBtn = new wxButton(topBar, wxID_ANY, "✕",
                                       wxDefaultPosition, wxSize(40, 40));
    closeBtn->SetBackgroundColour(wxColour(200, 0, 0));
    closeBtn->SetForegroundColour(*wxWHITE);
    closeBtn->SetWindowStyleFlag(wxBORDER_NONE);

    topBarSizer->Add(closeBtn, 0,  wxEXPAND, 5);
    topBar->SetSizer(topBarSizer);

    closeBtn->Bind(wxEVT_BUTTON, [frame](wxCommandEvent&) { frame->Destroy(); });

    wxSize screenSize = frame->GetClientSize();
    int topBarHeight = 50;
    int availableWidth = screenSize.GetWidth() - 40;
    int availableHeight = screenSize.GetHeight() - topBarHeight - 40;

    double scaleX = (double)availableWidth / img.GetWidth();
    double scaleY = (double)availableHeight / img.GetHeight();
    double scale = std::min(scaleX, scaleY);

    int newW = (int)(img.GetWidth() * scale);
    int newH = (int)(img.GetHeight() * scale);

    wxBitmap fullBitmap(img.Scale(newW, newH, wxIMAGE_QUALITY_HIGH));
    wxStaticBitmap* imgDisplay = new wxStaticBitmap(panel, wxID_ANY, fullBitmap);

    wxBoxSizer* mainVerticalSizer = new wxBoxSizer(wxVERTICAL);
    mainVerticalSizer->Add(topBar, 0, wxEXPAND);
    mainVerticalSizer->AddStretchSpacer(1);
    mainVerticalSizer->Add(imgDisplay, 0, wxALIGN_CENTER);
    mainVerticalSizer->AddStretchSpacer(1);

    panel->SetSizer(mainVerticalSizer);

    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(panel, 1, wxEXPAND);
    frame->SetSizer(frameSizer);


    panel->Bind(wxEVT_KEY_DOWN, [frame](wxKeyEvent& e) {
        if (e.GetKeyCode() == WXK_ESCAPE) frame->Destroy();
    });
    frame->Bind(wxEVT_CLOSE_WINDOW, [frame](wxCloseEvent&) { frame->Destroy(); });
    panel->SetFocus();
    frame->Layout();
}

Storage_Panel::~Storage_Panel() {

}
