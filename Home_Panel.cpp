//
// Created by evan on 2026-02-24.
//

#include "Home_Panel.h"

Home_Panel::Home_Panel(wxWindow *parent, SecuritySystem *system, UI *mainFrame)
    : wxPanel(parent, wxID_ANY), m_system(system), m_ui(mainFrame) {
    // 'this' is the panel. We put the sizer directly on 'this'.
    auto* panelSizer = new wxBoxSizer(wxVERTICAL);


    this->SetSizer(panelSizer);
}