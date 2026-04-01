//
// Created by evan on 2026-02-24.
//

#include "Home_Panel.h"

#include "Camera.h"
#include "SecuritySystem.h"
#include "Storage_Panel.h"
#include "UI.h"

/**
 * @brief Constructs the Home_Panel UI component.
 *
 * Initializes all UI elements including the camera placeholder, status label,
 * arm/disarm toggle, alarm button, and navigation buttons (Storage, Settings, Logout).
 *
 * @param parent The parent wxWindow.
 * @param system Pointer to the SecuritySystem for system control.
 * @param mainFrame Pointer to the main UI frame for panel navigation.
 */
Home_Panel::Home_Panel(wxWindow *parent, SecuritySystem *system, UI *mainFrame)
    : wxPanel(parent, wxID_ANY), m_system(system), m_ui(mainFrame) {

    wxFont buttonFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    auto* panelSizer = new wxBoxSizer(wxVERTICAL);
    auto* mainColumns = new wxBoxSizer(wxHORIZONTAL);

    panelSizer->AddStretchSpacer(1);

    auto* leftColumn = new wxBoxSizer(wxVERTICAL);
    statusLabel = new wxStaticText(this, wxID_ANY, "Status : Disarmed");
    wxBitmap placeholder(400, 300);
    cameraView = new wxStaticBitmap(this, wxID_ANY, placeholder);
    loadPlaceholder();
    actionBtn = new wxToggleButton(this, wxID_ANY, "ARM SYSTEM");
    alarmBtn = new wxToggleButton(this,wxID_ANY, "Sound Alarm");
    alarmBtn->SetFont(buttonFont);
    actionBtn->SetFont(buttonFont);
    actionBtn->SetForegroundColour(*wxGREEN);


    leftColumn->Add(statusLabel,0,wxEXPAND);
    leftColumn->Add(cameraView, 3, wxEXPAND | wxBOTTOM, 20);
    leftColumn->Add(actionBtn, 1, wxEXPAND);
    leftColumn->Add(alarmBtn,1,wxEXPAND);

    //btn font
    auto* rightColumn = new wxBoxSizer(wxVERTICAL);
    rightColumn->SetMinSize(wxSize(150, -1));

    wxButton* settingsBtn = new wxButton(this, wxID_ANY, "Settings", wxDefaultPosition, wxDefaultSize);
    wxButton* storageBtn = new wxButton (this, wxID_ANY, "Storage", wxDefaultPosition, wxDefaultSize);
    wxButton* logoutBtn = new wxButton (this, wxID_ANY, "Logout", wxDefaultPosition, wxDefaultSize);


    settingsBtn->SetFont(buttonFont);
    storageBtn->SetFont(buttonFont);
    logoutBtn->SetFont(buttonFont);
    storageBtn->InvalidateBestSize();
    storageBtn->SetSize(storageBtn->GetBestSize());
    logoutBtn->InvalidateBestSize();
    logoutBtn->SetSize(logoutBtn->GetBestSize());
    settingsBtn->InvalidateBestSize();
    settingsBtn->SetSize(settingsBtn->GetBestSize());

    rightColumn->Add(storageBtn, 1, wxEXPAND | wxBOTTOM, 20);
    rightColumn->Add(settingsBtn, 1, wxEXPAND | wxBOTTOM,20);
    rightColumn->Add(logoutBtn, 1, wxEXPAND ,20);


    mainColumns->AddStretchSpacer(1);      // Left margin
    mainColumns->Add(leftColumn, 2, wxEXPAND);
    mainColumns->AddSpacer(50);            // Gap between camera and buttons
    mainColumns->Add(rightColumn, 1, wxEXPAND);
    mainColumns->AddStretchSpacer(1);      // Right margin

    panelSizer->Add(mainColumns, 2, wxEXPAND);

    panelSizer->AddStretchSpacer(1);

    m_system->getCamera()->startStream();
    m_system->getCamera()->onFrame([this](const std::vector<uint8_t>& jpeg) {
        // Store latest frame — callback is on network thread
        std::lock_guard<std::mutex> lock(frameMutex);
        pendingFrame = jpeg;
    });

    // Timer polls for new frames on the main/UI thread
    frameTimer = new wxTimer(this);
    Bind(wxEVT_TIMER, &Home_Panel::updateFrame, this);
    frameTimer->Start(33);

    this->SetSizer(panelSizer);

    actionBtn->Bind(wxEVT_TOGGLEBUTTON,&Home_Panel::onArmButtonPressed,this);
    logoutBtn->Bind(wxEVT_BUTTON,&Home_Panel::onLogout,this);
    storageBtn->Bind(wxEVT_BUTTON,&Home_Panel::onStorageButtonPressed,this);
    alarmBtn->Bind(wxEVT_TOGGLEBUTTON,&Home_Panel::onAlarmButtonPressed,this);
    settingsBtn->Bind(wxEVT_BUTTON,&Home_Panel::onConfigButtonPressed,this);

    // wxButton* testBtn = new wxButton(this, wxID_ANY, "Test Alert");
    // testBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    //     std::string time = "2026-03-28 03:00";
    //     Event e(time, "Motion detected");
    //     Alert alert(e, *m_system);
    //     alert.sendAlert();
    // });
    
}

void Home_Panel::onAlarmTriggered() {
    alarmBtn->SetValue(true);
    alarmBtn->SetLabel("Turn Off Alarm");
}

void Home_Panel::onAlarmDisabled() {
    alarmBtn->SetValue(false);
    alarmBtn->SetLabel("Sound Alarm");

}


/**
 * @brief Handles the ARM/DISARM toggle button press.
 *
 * Toggles the system between armed and disarmed states, updating
 * the button label, colour, and status label accordingly.
 *
 * @param event The wxCommandEvent triggered by the toggle button.
 */
void Home_Panel::onArmButtonPressed(wxCommandEvent &event) {
    if (!m_isArmed) {
        actionBtn->SetValue(true);
        actionBtn->SetLabel("DISARM System");
        actionBtn->SetForegroundColour(*wxRED);
        statusLabel->SetLabel("Status: Armed");
        m_system->arm();
        m_isArmed = true;
    }
    else {
        actionBtn->SetValue(false);
        actionBtn->SetLabel("ARM SYSTEM");
        actionBtn->SetForegroundColour(*wxGREEN);
        statusLabel->SetLabel("Status: Disarmed");

        m_system->disarm();
        m_isArmed = false;
    }

}


/**
 * @brief Handles the Logout button press.
 *
 * Navigates the user back to the Login panel.
 *
 * @param event The wxCommandEvent triggered by the logout button.
 */
void Home_Panel::onLogout(wxCommandEvent &event) {
    m_ui->SwitchPage(UI::Login_ID);
}

void Home_Panel::loadPlaceholder() {
    wxMemoryDC dc;
    wxBitmap bmp(400, 300);
    dc.SelectObject(bmp);
    dc.SetBackground(wxBrush(wxColour(30, 30, 30)));
    dc.Clear();

    // White text centered
    dc.SetTextForeground(*wxWHITE);
    dc.SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxString msg = "No Camera Connected";
    wxSize textSize = dc.GetTextExtent(msg);
    int x = (400 - textSize.x) / 2;
    int y = (300 - textSize.y) / 2;
    dc.DrawText(msg, x, y);


    dc.SelectObject(wxNullBitmap);
    cameraView->SetBitmap(bmp);
    cameraView->Refresh();
    cameraView->SetBackgroundColour(wxColour(30, 30, 30));
    cameraView->SetMinSize(wxSize(400, 400));

}

void Home_Panel::updateFrame(wxTimerEvent&) {

    if (!cameraConnected) {
        // Check if connection was established since last check
        if (m_system->isConnected()) {
            cameraConnected = true;
            m_system->getCamera()->startStream();
            m_system->getCamera()->onFrame([this](const std::vector<uint8_t>& jpeg) {
                std::lock_guard<std::mutex> lock(frameMutex);
                pendingFrame = jpeg;
            });
        }
        return;
    }

    // If we lost connection mid-stream
    if (!m_system->isConnected()) {
        cameraConnected = false;
        loadPlaceholder();
        return;
    }

    std::vector<uint8_t> frameToShow;
    {
        std::lock_guard<std::mutex> lock(frameMutex);
        if (pendingFrame.empty()) return;
        frameToShow = std::move(pendingFrame);
    }

    // Decode JPEG bytes
    cv::Mat frame = cv::imdecode(frameToShow, cv::IMREAD_COLOR);
    if (frame.empty()) return;

    // Convert BGR to RGB for wxImage
    cv::Mat rgb;
    cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);

    // Resize to fit the display
    wxSize displaySize = cameraView->GetSize();
    cv::resize(rgb, rgb, cv::Size(displaySize.x, displaySize.y));

    // Convert to wxBitmap and display
    wxImage img(rgb.cols, rgb.rows, rgb.data, true);
    cameraView->SetBitmap(wxBitmap(img));
    cameraView->Refresh();
}


/**
 * @brief Handles the Storage button press.
 *
 * Navigates to the Storage panel and triggers image loading.
 *
 * @param event The wxCommandEvent triggered by the storage button.
 */
void Home_Panel::onStorageButtonPressed(wxCommandEvent &event) {
    m_ui->SwitchPage(UI::Storage_ID);
    m_ui->getStoragePanel()->loadImages();
    m_ui->getStoragePanel()->Layout();
}

void Home_Panel::onConfigButtonPressed(wxCommandEvent &event) {
    m_ui->SwitchPage(UI::Setting_ID);
}

/**
 * @brief Handles the Sound Alarm toggle button press.
 *
 * @param event The wxCommandEvent triggered by the alarm button.
 * @todo Implement alarm sound functionality.
 */
void Home_Panel::onAlarmButtonPressed(wxCommandEvent &event) {
    if (!m_system->getIsAlarmActive()) {
       onAlarmTriggered();
        m_system->soundAlarm();
    }
    else {
        alarmBtn->SetValue(false);
        alarmBtn->SetLabel("Sound Alarm");
        m_system->turnOffAlarm();
    }
}

/**
 * @brief Destructor for Home_Panel.
 */
Home_Panel::~Home_Panel() {
    frameTimer->Stop();
    m_system->getCamera()->stopStream();
}


