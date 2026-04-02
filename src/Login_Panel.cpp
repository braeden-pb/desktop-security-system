/**
* @file Login_Panel.cpp
 * @brief Implementation of the Login_Panel class, the authentication UI panel
 *        for the security system.
 * @author evan
 * @date 2026-03-06
 */

#include "Login_Panel.h"


/**
 * @brief Constructs the Login_Panel UI component.
 *
 * Initializes the login screen with a title, PIN input field,
 * and a login button. The panel is vertically and horizontally
 * centered within the window.
 *
 * @param parent The parent wxWindow.
 * @param system Pointer to the SecuritySystem for PIN validation.
 * @param mainFrame Pointer to the main UI frame for panel navigation.
 */
Login_Panel::Login_Panel(wxWindow *parent, SecuritySystem *system, UI *mainFrame)
    : wxPanel(parent, wxID_ANY), m_system(system), m_ui(mainFrame),loginSuccessful(false) {


    loginSuccessful = false;
    auto* panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->AddStretchSpacer(1);

    auto* titleText = new wxStaticText(this, wxID_ANY, "Security System Login");
    wxFont titleFont(28, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    wxSize trueSize = titleText->GetBestSize();
    titleText->SetMinSize(wxSize(trueSize.x + 10, trueSize.y));
    panelSizer->Add(titleText, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 20);
    panelSizer->AddStretchSpacer(2);


    auto* horizontalCenterer = new wxBoxSizer(wxHORIZONTAL);
    horizontalCenterer->AddStretchSpacer(1);

    auto* mainColumn = new wxBoxSizer(wxVERTICAL);
    mainColumn->SetMinSize(wxSize(300, 200));

    auto* passLabel = new wxStaticText(this, wxID_ANY, "Enter System PIN:");
    pinInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD | wxTE_CENTRE | wxTE_PROCESS_ENTER);           pinInput->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    loginBtn = new wxButton(this, wxID_ANY, "LOGIN");

    mainColumn->Add(passLabel, 0, wxALIGN_CENTER | wxTOP, 20);
    mainColumn->Add(pinInput, 0, wxALIGN_CENTER | wxTOP, 10);
    mainColumn->Add(loginBtn, 0, wxALIGN_CENTER | wxTOP, 30);
    horizontalCenterer->Add(mainColumn,0,wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    horizontalCenterer->AddStretchSpacer(1);

    panelSizer->Add(horizontalCenterer, 1, wxEXPAND | wxALL, 40);
    panelSizer->AddStretchSpacer(4);


    this->SetSizer(panelSizer);

    loginBtn->Bind(wxEVT_BUTTON, &Login_Panel::onLogin, this);
    pinInput->Bind(wxEVT_TEXT_ENTER, &Login_Panel::onLogin, this);
}

/**
 * @brief Handles the LOGIN button press.
 *
 * Validates the entered PIN via the SecuritySystem. On success,
 * navigates to the Home panel and clears the input field.
 * On failure, displays an error dialog.
 *
 * @param event The wxCommandEvent triggered by the login button.
 */
void Login_Panel::onLogin(wxCommandEvent &event) {
    if (m_system->validatePIN(std::string(pinInput->GetValue()))) {
        m_ui->SwitchPage(UI::Home_ID);
        pinInput->Clear();
        loginSuccessful = true;
    } else {
        wxMessageBox("Incorrect PIN", "Access Denied", wxOK | wxICON_ERROR);
    }
}

/**
 * @brief Returns whether the last login attempt was successful.
 *
 * Reflects the result of the most recent PIN validation attempt.
 * Returns false by default until a successful login occurs.
 *
 * @return True if the last login attempt succeeded, false otherwise.
 */
bool Login_Panel::isLoginSuccessful() const {
    return loginSuccessful;
}

/**
 * @brief Destructor for Login_Panel.
 */
Login_Panel::~Login_Panel() {

}
