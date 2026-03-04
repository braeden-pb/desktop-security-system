//
// Created by evan on 2026-02-23.
//

#include "UI.h"
#include "Home_Panel.h"

UI::UI(SecuritySystem* system) :
wxFrame(nullptr, wxID_ANY, "Security System", wxDefaultPosition),
m_system(system)

{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    book = new wxSimplebook(this, wxID_ANY);
    Home_Panel* homePage = new Home_Panel(book, system,this);
    book->AddPage(homePage, "Home");
    mainSizer->Add(book, 1, wxEXPAND);

    SetSizer(mainSizer);

    Centre();





}

UI::~UI() {}
