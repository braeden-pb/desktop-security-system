//
// Created by evan on 2026-03-08.
//

#include "Config_Panel.h"

Config_Panel::Config_Panel(wxWindow *parent, SecuritySystem *system, UI *mainFrame) :
wxPanel(parent, wxID_ANY), m_system(system), m_ui(mainFrame) {
    wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText3 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("Change Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer3->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_textCtrl2 = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_textCtrl2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_button1 = new wxButton( m_scrolledWindow1, wxID_ANY, _("Set Password"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticText5 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("Adjust Clip Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer3->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_slider1 = new wxSlider( m_scrolledWindow1, wxID_ANY, 5, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer3->Add( m_slider1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer2->Add( bSizer3, 1, wxEXPAND, 10 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_staticText4 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("Add Authorized Face"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer4->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_filePicker1 = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, _("Upload A File"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SAVE );
	bSizer4->Add( m_filePicker1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticText6 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("Capture Images/Videos?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer4->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxString m_radioBox1Choices[] = { _("Yes"), _("No") };
	int m_radioBox1NChoices = sizeof( m_radioBox1Choices ) / sizeof( wxString );
	m_radioBox1 = new wxRadioBox( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_radioBox1NChoices, m_radioBox1Choices, 1, wxRA_SPECIFY_ROWS );
	m_radioBox1->SetSelection( 1 );
	bSizer4->Add( m_radioBox1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer2->Add( bSizer4, 1, wxEXPAND, 5 );


	m_scrolledWindow1->SetSizer( bSizer2 );
	m_scrolledWindow1->Layout();
	bSizer2->Fit( m_scrolledWindow1 );
	bSizer1->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 10 );

	m_button2 = new wxButton( this, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_button2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

Config_Panel::~Config_Panel() {}
