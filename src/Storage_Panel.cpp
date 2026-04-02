/**
* @file Storage_Panel.cpp
 * @brief Implementation of the storage gallery panel for the security system.
 * @author evan
 * @date 2026-03-08
 */

#include "Storage_Panel.h"

/**
 * @brief Constructs the Storage_Panel UI component.
 *
 * Initializes the gallery layout, scrollable image grid, top bar
 * with title and back button, and binds all event handlers.
 *
 * @param parent The parent wxWindow.
 * @param system Pointer to the SecuritySystem for accessing stored images.
 * @param ui Pointer to the main UI frame for panel navigation.
 */
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

    errorLabel = new wxStaticText(this, wxID_ANY, "");

    outerSizer->Add(errorLabel, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 50);
    errorLabel->Hide();
    homeBtn->Bind(wxEVT_BUTTON,&Storage_Panel::onBackHome,this);

    this->SetSizer(outerSizer);

}

/**
 * @brief Loads and scales a thumbnail from a file path.
 *
 * Scales the image to fit within a 250x250 bounding box while
 * maintaining aspect ratio. Suppresses wx error dialogs for invalid files.
 *
 * @param path The file path of the image to load.
 * @return A scaled wxBitmap, or wxNullBitmap if the image failed to load.
 */
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

/**
 * @brief Loads all images from storage and populates the gallery grid.
 *
 * Clears any existing thumbnails, fetches the latest image list from
 * the SecuritySystem, and creates a thumbnail panel for each image
 * with a delete button overlay and timestamp label.
 * Displays an error label if no images are found.
 */
void Storage_Panel::loadImages() {
    errorLabel->Show(false);
    gridSizer->Clear(true);


    auto imageList = m_system->getAllImages();

    if (imageList.empty()) {
        errorLabel->SetLabel("No Images or Videos");
        errorLabel->Show(true);
        return;
    }
    for (const auto& [id,time,path] : imageList) {
        wxBitmap thumbnail = loadThumbnail(path);
        std::string lowerPath{wxString(path).Lower()};
        if (!thumbnail.IsOk()) {
            if (!m_system->getStorage()->isVideoFile(lowerPath)) continue;
            thumbnail = wxBitmap(250, 250);
            wxMemoryDC dc(thumbnail);
            dc.SetBackground(wxBrush(wxColour(30, 30, 30)));
            dc.Clear();
            dc.SetTextForeground(*wxWHITE);
            dc.DrawText("Video", wxPoint(90, 120));
        }

        wxBoxSizer* itemSizer = new wxBoxSizer(wxVERTICAL);

        wxPanel* itemPanel = new wxPanel(galleryPanel, wxID_ANY);
        itemPanel->SetMinSize(wxSize(250, 300));

        wxPanel* thumbContainer = new wxPanel(itemPanel, wxID_ANY);
        thumbContainer->SetMinSize(wxSize(250, 250));

        wxStaticBitmap* thumb = new wxStaticBitmap(thumbContainer, wxID_ANY, thumbnail);
        thumb->SetPosition(wxPoint(0, 0));
        thumb->SetSize(wxSize(250, 250));
        thumb->Bind(wxEVT_LEFT_DOWN, [this,path,lowerPath](wxMouseEvent&) { if (m_system->getStorage()->isVideoFile(lowerPath))
            openVideo(path);
        else
            OpenFullImage(path);
        });

        itemSizer->Add(thumbContainer, 0, wxEXPAND);

        wxStaticText* timeLabel = new wxStaticText(itemPanel, wxID_ANY, time);
        timeLabel->SetForegroundColour(wxColour(180, 180, 180));
        wxFont labelFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        timeLabel->SetFont(labelFont);
        itemSizer->Add(timeLabel, 0, wxALIGN_CENTER | wxTOP, 4);

        wxButton* deleteBtn = new wxButton(itemPanel, wxID_ANY, "x",
            wxPoint(210, 5), wxSize(35, 35));
        deleteBtn->SetBackgroundColour(wxColour(180, 30, 30));
        deleteBtn->SetForegroundColour(*wxWHITE);
        deleteBtn->SetWindowStyleFlag(wxBORDER_NONE);

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

        itemPanel->SetSizer(itemSizer);
        gridSizer->Add(itemPanel, 1, wxEXPAND | wxALL, 5);
    }
    galleryPanel->FitInside();
    galleryPanel->Layout();
    outerSizer->Layout();
    this->Layout();
}

/**
 * @brief Handles the Back Home button press.
 *
 * Navigates back to the Home panel.
 *
 * @param event The wxCommandEvent triggered by the back home button.
 */
void Storage_Panel::onBackHome(wxCommandEvent &event) {
    m_ui->SwitchPage(UI::Home_ID);
}

/**
 * @brief Handles gallery panel resize events.
 *
 * Dynamically adjusts the number of grid columns based on the
 * available panel width to keep thumbnails properly laid out.
 *
 * @param event The wxSizeEvent triggered on panel resize.
 */
void Storage_Panel::OnGalleryResize(wxSizeEvent& event) {
    int panelWidth = event.GetSize().GetWidth();
    int thumbSize = 265;
    int cols = std::max(1, panelWidth / thumbSize);

    gridSizer->SetCols(cols);
    galleryPanel->FitInside();
    galleryPanel->Layout();
    event.Skip();
}

/**
 * @brief Opens a full-screen image viewer for the selected image.
 *
 * Creates a new fullscreen wxFrame displaying the image scaled to fit
 * the screen. Includes a close button and ESC key support to dismiss.
 * Uses wxDisplay to get accurate screen dimensions for correct scaling.
 *
 * @param path The file path of the image to display.
 */
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

    wxDisplay display(wxDisplay::GetFromWindow(this));
    wxRect screen = display.GetClientArea();
    int topBarHeight = 50;
    int availableWidth = screen.GetWidth() - 40;
    int availableHeight = screen.GetHeight() - topBarHeight - 40;

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

/**
 * @brief Launches an external video player for the selected file.
 *
 * Uses wxExecute to launch VLC media player as a background process.
 *
 * @param path The file path of the video to open.
 */
void Storage_Panel::openVideo(const wxString& path) {
    wxExecute("vlc \"" + path + "\"", wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER);
}

/**
 * @brief Destructor for Storage_Panel.
 */
Storage_Panel::~Storage_Panel() {}
