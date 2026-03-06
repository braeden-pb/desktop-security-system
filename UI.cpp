//
// Created by evan on 2026-02-23.
//

#include "UI.h"
#include "Home_Panel.h"
#include "Login_Panel.h"

UI::UI(SecuritySystem* system) :
wxFrame(nullptr, wxID_ANY, "Security System", wxDefaultPosition),
m_system(system),
menuBar(nullptr)
{

    menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_ABOUT, "&About");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "&Exit");

    menuBar->Append(fileMenu,"&File");

    wxFrameBase::SetMenuBar(menuBar);


    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    book = new wxSimplebook(this, wxID_ANY);
    auto* homePage = new Home_Panel(book, system,this);
    auto loginPage = new Login_Panel(book,system,this);
    book->AddPage(loginPage, "Login");
    book->AddPage(homePage, "Home");
    mainSizer->Add(book, 1, wxEXPAND);

    SetSizer(mainSizer);

    this->SetMinSize(wxSize(600, 400));

    Centre();

    Bind(wxEVT_MENU, &UI::onExit, this, wxID_EXIT);






}

void UI::SwitchPage(PageID id) {
    if (book) {
        book->SetSelection(id);
    }

    this->Layout();

}

void UI::onExit(wxCommandEvent& event) {
    exit(0);
}

UI::PageID UI::GetPageID() {

}

UI::~UI() {}
