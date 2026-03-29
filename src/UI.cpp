//
// Created by evan on 2026-02-23.
//

#include "UI.h"
#include "Home_Panel.h"
#include "Login_Panel.h"
#include "Storage_Panel.h"


/**
 * @brief Constructs the main UI frame.
 *
 * Initializes the menu bar, page book, and all panels (Login, Home, Storage).
 * Sets the minimum window size, centres the frame on screen, and binds
 * the exit menu event.
 *
 * @param system Pointer to the SecuritySystem shared across all panels.
 */
UI::UI(SecuritySystem* system) :
wxFrame(nullptr, wxID_ANY, "Security System", wxDefaultPosition),
m_system(system),
menuBar(new wxMenuBar())
{
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_ABOUT, "&About");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "&Exit");

    menuBar->Append(fileMenu,"&File");

    SetMenuBar(menuBar);


    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    book = new wxSimplebook(this, wxID_ANY);
    homePage = new Home_Panel(book, system,this);
    loginPage = new Login_Panel(book,system,this);
    storagePage = new Storage_Panel(book,system,this);
    configPage = new Config_Panel(book,system,this);
    book->AddPage(loginPage, "Login");
    book->AddPage(homePage, "Home");
    book->AddPage(storagePage,"Storage");
    book->AddPage(configPage,"Config");
    mainSizer->Add(book, 1, wxEXPAND);

    SetSizer(mainSizer);

    this->SetMinSize(wxSize(600, 600));

    Centre();

    Bind(wxEVT_MENU, &UI::onExit, this, wxID_EXIT);






}

/**
 * @brief Switches the visible page in the wxSimplebook.
 *
 * Uses the PageID enum to select the correct panel and triggers
 * a layout refresh on the frame.
 *
 * @param id The PageID of the panel to display.
 */
void UI::SwitchPage(PageID id) {
    if (book) {
        book->SetSelection(id);
    }

    this->Layout();

}

/**
 * @brief Handles the Exit menu item event.
 *
 * Initiates a clean wxWidgets shutdown by closing the frame.
 *
 * @param event The wxCommandEvent triggered by the Exit menu item.
 */
void UI::onExit(wxCommandEvent& event) {
    Close(true);
}

/**
 * @brief Returns the currently active page ID.
 *
 * Queries the wxSimplebook for its current selection and casts
 * it to the PageID enum.
 *
 * @return The PageID of the currently displayed panel.
 */
UI::PageID UI::GetPageID() const {
    return static_cast<PageID>(book->GetSelection());
}

/**
 * @brief Returns a pointer to the Storage panel.
 *
 * Used externally to trigger image loading when navigating
 * to the Storage page.
 *
 * @return Pointer to the Storage_Panel instance.
 */
Storage_Panel* UI::getStoragePanel() const {
    return storagePage;
}

void UI::showAlert(const std::string& message)
    {
    wxMessageBox(message, "Alert", wxOK | wxICON_WARNING, this);
    }

void UI::update(const std::string& event) {
    //showAlert(event);
}

UI::~UI() {}
