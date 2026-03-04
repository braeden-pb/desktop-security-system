//
// Created by evan on 2026-02-24.
//

#include "Home_Panel.h"

Home_Panel::Home_Panel(wxWindow *parent, SecuritySystem *system, UI *mainFrame)
    : wxPanel(parent, wxID_ANY), m_system(system), m_ui(mainFrame) {
    // 'this' is the panel. We put the sizer directly on 'this'.
    auto* panelSizer = new wxBoxSizer(wxVERTICAL);

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
}

Home_Panel::~Home_Panel() {}