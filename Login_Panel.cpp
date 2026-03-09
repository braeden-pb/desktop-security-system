//
// Created by evan on 2026-03-06.
//

#include "Login_Panel.h"

Login_Panel::Login_Panel(wxWindow *parent, SecuritySystem *system, UI *mainFrame)
    : wxPanel(parent, wxID_ANY), m_system(system), m_ui(mainFrame) {

    auto* panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->AddStretchSpacer(1);

    auto* titleText = new wxStaticText(this, wxID_ANY, "Securtiy System Login");
    wxFont titleFont(28, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->InvalidateBestSize();
    titleText->SetMinSize(titleText->GetBestSize());
    panelSizer->Add(titleText, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 20);
    panelSizer->AddStretchSpacer(2);


    auto* horizontalCenterer = new wxBoxSizer(wxHORIZONTAL);
    horizontalCenterer->AddStretchSpacer(1);

    auto* mainColumn = new wxBoxSizer(wxVERTICAL);
    mainColumn->SetMinSize(wxSize(300, 200));

    auto* passLabel = new wxStaticText(this, wxID_ANY, "Enter System PIN:");
    auto* pinInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD | wxTE_CENTRE);
    pinInput->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    auto* loginBtn = new wxButton(this, wxID_ANY, "LOGIN");

    mainColumn->Add(passLabel, 0, wxALIGN_CENTER | wxTOP, 20);
    mainColumn->Add(pinInput, 0, wxALIGN_CENTER | wxTOP, 10);
    mainColumn->Add(loginBtn, 0, wxALIGN_CENTER | wxTOP, 30);
    horizontalCenterer->Add(mainColumn,0,wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    horizontalCenterer->AddStretchSpacer(1);

    panelSizer->Add(horizontalCenterer, 1, wxEXPAND | wxALL, 40);
    panelSizer->AddStretchSpacer(4);


    this->SetSizer(panelSizer);

}

void Login_Panel::onLogin(wxCommandEvent &event) {

}

Login_Panel::~Login_Panel() {

}
