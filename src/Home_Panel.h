//
// Created by evan on 2026-02-24.
//

#ifndef GROUP55_HOME_PANEL_H
#define GROUP55_HOME_PANEL_H

#include <mutex>
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <opencv2/opencv.hpp>


class UI;
class SecuritySystem;


/**
 * @brief Panel that serves as the main home screen of the security system.
 *
 * Displays the camera feed placeholder, system arm/disarm controls,
 * alarm button, and navigation to Storage and Settings panels.
 */
class Home_Panel : public wxPanel {
public:
    /**
     * @brief Constructs the Home_Panel.
     * @param parent The parent wxWindow.
     * @param system Pointer to the SecuritySystem for system control.
     * @param mainFrame Pointer to the main UI frame for panel navigation.
     */
    Home_Panel(wxWindow* parent, SecuritySystem* system, UI* mainFrame);

    ~Home_Panel();

    void onAlarmTriggered();
    void onAlarmDisabled();

private:
    bool m_isArmed = false;        ///< Tracks whether the system is currently armed
    SecuritySystem* m_system;      ///< Pointer to the security system
    UI* m_ui;                      ///< Pointer to the main UI frame
    wxToggleButton* actionBtn;     ///< The ARM/DISARM toggle button
    wxStaticText* statusLabel;     ///< Displays the current system status
    wxToggleButton* alarmBtn;
    wxStaticBitmap* cameraView;
    wxTimer* frameTimer;
    std::vector<uint8_t> pendingFrame;
    std::mutex frameMutex;
    bool cameraConnected = false;

    void updateFrame(wxTimerEvent& event);
    void loadPlaceholder();


    /**
    * @brief Handles the Config button press, navigating to the Config panel.
    * @param event The button event.
    */
    void onConfigButtonPressed(wxCommandEvent& event);

    /**
     * @brief Toggles the system between armed and disarmed states.
     * @param event The toggle button event.
     */
    void onArmButtonPressed(wxCommandEvent& event);

    /**
     * @brief Navigates back to the Login panel.
     * @param event The button event.
     */
    void onLogout(wxCommandEvent& event);

    /**
     * @brief Navigates to the Storage panel and loads images.
     * @param event The button event.
     */
    void onStorageButtonPressed(wxCommandEvent& event);

    /**
     * @brief Handles the alarm button press.
     * @param event The button event.
     * @todo Implement alarm sound functionality.
     */
    void onAlarmButtonPressed(wxCommandEvent& event);

    /**
     * @brief Returns the password input control.
     * @return Pointer to the password wxTextCtrl.
     */
};


#endif //GROUP55_HOME_PANEL_H
