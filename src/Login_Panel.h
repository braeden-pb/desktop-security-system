//
// Created by evan on 2026-03-06.
//

#ifndef GROUP55_LOGIN_PANEL_H
#define GROUP55_LOGIN_PANEL_H

#include <wx/wx.h>
#include "SecuritySystem.h"
#include "UI.h"

class UI;
class SecuritySystem;

/**
 * @brief Panel that provides the login screen for the security system.
 *
 * Displays a PIN input field and login button. Validates the entered
 * PIN against the SecuritySystem and navigates to the Home panel on success.
 */
class Login_Panel : public wxPanel {
public:
    /**
     * @brief Constructs the Login_Panel.
     * @param parent The parent wxWindow.
     * @param system Pointer to the SecuritySystem for PIN validation.
     * @param mainFrame Pointer to the main UI frame for panel navigation.
     */
    Login_Panel(wxWindow* parent, SecuritySystem* system, UI* mainFrame);

    ~Login_Panel();

    /**
      * @brief Returns the password input control.
      * @return Pointer to the PIN input wxTextCtrl as a wxWindow.
      */
    wxWindow* GetPasswordCtrl();

    /**
     * @brief Returns the login button control.
     * @return Pointer to the login wxButton as a wxWindow.
     */
    wxWindow* GetLoginButton();

    /**
     * @brief Returns whether the last login attempt was successful.
     * @return True if login was successful, false otherwise.
     */
    bool isLoginSuccessful() const;

private:
    SecuritySystem* m_system;  ///< Pointer to the security system for PIN validation
    UI* m_ui;                  ///< Pointer to the main UI frame for navigation
    wxTextCtrl* pinInput;      ///< The PIN input text field
    wxButton* loginBtn;        ///< The login button
    bool loginSuccessful;      ///< Tracks whether the last login attempt succeeded

    /**
     * @brief Handles the LOGIN button press event.
     * @param event The wxCommandEvent triggered by the login button.
     */
    void onLogin(wxCommandEvent& event);
};


#endif //GROUP55_LOGIN_PANEL_H
