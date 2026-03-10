//
// Created by evan on 2026-02-24.
//

#include "Home_Panel.h"

#include "SecuritySystem.h"
#include "UI.h"

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
    actionBtn->SetFont(buttonFont);
    actionBtn->SetForegroundColour(*wxGREEN);


    leftColumn->Add(statusLabel,0,wxEXPAND);
    leftColumn->Add(cameraPlaceholder, 3, wxEXPAND | wxBOTTOM, 20); // Camera gets more space
    leftColumn->Add(actionBtn, 1, wxEXPAND);

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



    auto* horizontalCenterer = new wxBoxSizer(wxHORIZONTAL);

    panelSizer->AddStretchSpacer(1);
    horizontalCenterer->AddStretchSpacer(1);

    //min btnSize
    wxSize btnSize(100, 40);

    //btn font
    wxFont buttonFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    auto* buttonStack = new wxBoxSizer(wxVERTICAL);


    wxButton* settingsBtn = new wxButton(this, wxID_ANY, "Settings", wxDefaultPosition, btnSize);

    wxButton* storageBtn = new wxButton (this, wxID_ANY, "Storage", wxDefaultPosition, btnSize);

    wxButton* viewCameraBtn = new wxButton (this, wxID_ANY, "Camera", wxDefaultPosition, btnSize);

    storageBtn->SetFont(buttonFont);
    viewCameraBtn->SetFont(buttonFont);
    settingsBtn->SetFont(buttonFont);

    buttonStack->Add(storageBtn, 1, wxEXPAND|wxALL, 0);
    buttonStack->AddSpacer(20);
    buttonStack->Add(viewCameraBtn, 1, wxEXPAND|wxALL, 0);
    buttonStack->AddSpacer(20);
    buttonStack->Add(settingsBtn, 1, wxEXPAND|wxALL, 0);

    horizontalCenterer->Add(buttonStack,1,wxALIGN_CENTER);
    horizontalCenterer->AddStretchSpacer(1);

    panelSizer->Add(horizontalCenterer,1, wxEXPAND);

    panelSizer->AddStretchSpacer(1);




    this->SetSizer(panelSizer);

    actionBtn->Bind(wxEVT_TOGGLEBUTTON,&Home_Panel::onArmButtonPressed,this);
    logoutBtn->Bind(wxEVT_BUTTON,&Home_Panel::onLogout,this);
    storageBtn->Bind(wxEVT_BUTTON,&Home_Panel::onStorageButtonPressed,this);
}

void Home_Panel::onArmButtonPressed(wxCommandEvent &event) {
    if (m_isArmed == false) {
        actionBtn->SetValue(true);
        actionBtn->SetLabel("DISARM System");
        actionBtn->SetForegroundColour(*wxRED);
        statusLabel->SetLabel("Status: Armed");
        m_system->setStatus(Status::armed);
        m_isArmed = true;
    }
    else {
        actionBtn->SetValue(false);
        actionBtn->SetLabel("ARM SYSTEM");
        actionBtn->SetForegroundColour(*wxGREEN);
        statusLabel->SetLabel("Status: Disarmed");

        m_system->setStatus(Status::disarmed);
        m_isArmed = false;
    }

}

void Home_Panel::onLogout(wxCommandEvent &event) {
    m_ui->SwitchPage(UI::Login_ID);
}

void Home_Panel::onStorageButtonPressed(wxCommandEvent &event) {
    m_ui->SwitchPage(UI::Storage_ID);
}

Home_Panel::~Home_Panel() {}