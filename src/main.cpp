/**
* @file main.cpp
 * @brief Application entry point for the PC-side security system UI.
 * @author evan
 * @date 2026-02-19
 */

#include <wx/wx.h>
#include "UI.h"
#include "SecuritySystem.h"

/**
 * @brief The top-level wxWidgets application class.
 *
 * Inherits from wxApp and overrides OnInit() to bootstrap the
 * SecuritySystem and launch the main UI window.
 */
class MyApp : public wxApp {
public:
    /**
    * @brief Initializes the application and launches the main UI window.
    *
    * Constructs the SecuritySystem, which internally creates and owns the UI
    * frame. Initializes all wxWidgets image handlers to enable JPEG and PNG
    * decoding (required for camera frame display), then shows and maximizes
    * the main window.
    *
    * @return true to indicate successful initialization and begin the event loop.
    */
    bool OnInit() override {
        auto* system = new SecuritySystem();
        UI* mainUI = system->getUI();
        wxInitAllImageHandlers();
        mainUI->Show(true);
        mainUI->Maximize(true);
        return true;
    }
};
/**
 * @brief wxWidgets application entry point macro.
 *
 * Replaces the standard main() function, instantiating MyApp and handing
 * control to the wxWidgets event loop.
 */
wxIMPLEMENT_APP(MyApp);
