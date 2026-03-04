//
// Created by evan on 2026-02-23.
//

#include "UI.h"

UI::UI(SecuritySystem* system) :
wxFrame(nullptr, wxID_ANY, "Security System", wxDefaultPosition)

{

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    book = new wxSimplebook(this, wxID_ANY);



}
