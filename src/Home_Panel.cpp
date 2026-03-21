//
// Created by evan on 2026-02-24.
//

#include "Home_Panel.h"

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
    auto* cameraPlaceholder = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
    cameraPlaceholder->SetMinSize(wxSize(200, 150));
    cameraPlaceholder->SetBackgroundColour(wxColour(30, 30, 30)); // Dark grey placeholder
    actionBtn = new wxToggleButton(this, wxID_ANY, "ARM SYSTEM");
    alarmBtn = new wxToggleButton(this,wxID_ANY, "Sound Alarm");
    alarmBtn->SetFont(buttonFont);
    actionBtn->SetFont(buttonFont);
    actionBtn->SetForegroundColour(*wxGREEN);


    leftColumn->Add(statusLabel,0,wxEXPAND);
    leftColumn->Add(cameraPlaceholder, 3, wxEXPAND | wxBOTTOM, 20); // Camera gets more space
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




    this->SetSizer(panelSizer);

    actionBtn->Bind(wxEVT_TOGGLEBUTTON,&Home_Panel::onArmButtonPressed,this);
    logoutBtn->Bind(wxEVT_BUTTON,&Home_Panel::onLogout,this);
    storageBtn->Bind(wxEVT_BUTTON,&Home_Panel::onStorageButtonPressed,this);
    alarmBtn->Bind(wxEVT_TOGGLEBUTTON,&Home_Panel::onAlarmButtonPressed,this);
    settingsBtn->Bind(wxEVT_BUTTON,&Home_Panel::onConfigButtonPressed,this);


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
        alarmBtn->SetValue(true);
        alarmBtn->SetLabel("Turn Off Alarm");
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
Home_Panel::~Home_Panel() {}