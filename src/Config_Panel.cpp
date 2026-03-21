//
// Created by evan on 2026-03-08.
//

#include "Config_Panel.h"

#include "Config.h"
#include "SecuritySystem.h"
#include "UI.h"

Config_Panel::Config_Panel(wxWindow *parent, SecuritySystem *system, UI *mainFrame) :
wxPanel(parent, wxID_ANY), m_system(system), m_ui(mainFrame), changesMade(false),changesSaved(false) {
    wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

    homeBtn = new wxButton( this, wxID_ANY, _("< Home"), wxDefaultPosition, wxSize(80, 30), 0 );

    topSizer->Add( homeBtn, 0, wxALL, 5 );
    topSizer->AddStretchSpacer(1); // pushes button to the left

    outerSizer->Add( topSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

    title = new wxStaticText( this, wxID_ANY, _("Settings"), wxDefaultPosition, wxDefaultSize, 0 );
    wxFont titleFont(28, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    title->SetFont(titleFont);
    title->Wrap( -1 );
    outerSizer->Add( title, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    innerScrollWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
    innerScrollWindow->SetScrollRate( 5, 5 );
    wxBoxSizer *windowSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *windowSizer2 = new wxBoxSizer(wxVERTICAL);

    PasswordBtn    = new wxButton( innerScrollWindow, wxID_ANY, _("Change Password"),           wxDefaultPosition, wxSize(-1, 50), 0 );
    ClipLenBtn     = new wxButton( innerScrollWindow, wxID_ANY, _("Photo/Image Settings"),      wxDefaultPosition, wxSize(-1, 50), 0 );
    motionBtn      = new wxButton( innerScrollWindow, wxID_ANY, _("Adjust Motion Sensitivity"), wxDefaultPosition, wxSize(-1, 50), 0 );
    alarmConfigBtn = new wxButton( innerScrollWindow, wxID_ANY, _("Alarm Settings"),            wxDefaultPosition, wxSize(-1, 50), 0 );

    windowSizer2->Add( PasswordBtn,    0, wxLEFT|wxRIGHT|wxTOP|wxEXPAND, 10 );
    windowSizer2->Add( ClipLenBtn,     0, wxLEFT|wxRIGHT|wxTOP|wxEXPAND, 10 );
    windowSizer2->Add( motionBtn,      0, wxLEFT|wxRIGHT|wxTOP|wxEXPAND, 10 );
    windowSizer2->Add( alarmConfigBtn, 0, wxLEFT|wxRIGHT|wxTOP|wxEXPAND, 10 );


    windowSizer->Add( windowSizer2, 1, wxALIGN_CENTER_VERTICAL, 5 );


    innerScrollWindow->SetSizer( windowSizer );
    innerScrollWindow->Layout();
    windowSizer->Fit( innerScrollWindow );
    outerSizer->Add( innerScrollWindow, 1, wxALL|wxEXPAND, 10 );
    outerSizer->AddStretchSpacer(0);


    SaveBtn = new wxButton( this, wxID_ANY, _("Save"), wxDefaultPosition, wxSize(120, 35), 0 );
    outerSizer->Add( SaveBtn, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, 15 );

    PasswordBtn->Bind(wxEVT_BUTTON,&Config_Panel::onChangePass,this);
    alarmConfigBtn->Bind(wxEVT_BUTTON,&Config_Panel::onChangeAlarm,this);
    ClipLenBtn->Bind(wxEVT_BUTTON,&Config_Panel::onChangeClipLen,this);
    motionBtn->Bind(wxEVT_BUTTON,&Config_Panel::onChangeMotion,this);
    SaveBtn->Bind(wxEVT_BUTTON,&Config_Panel::onSave,this);
    homeBtn->Bind(wxEVT_BUTTON,&Config_Panel::onHomeButton,this);




    this->SetSizer( outerSizer );
    this->Layout();
}

void Config_Panel::onChangePass(wxCommandEvent &event) {
    wxDialog dlg(this, wxID_ANY, "Change Password", wxDefaultPosition, wxSize(350, 280));
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(new wxStaticText(&dlg, wxID_ANY, "Current Password:"), 0, wxLEFT|wxRIGHT|wxTOP, 15);
    wxTextCtrl* currentPass = new wxTextCtrl(&dlg, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    mainSizer->Add(currentPass, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    mainSizer->Add(new wxStaticText(&dlg, wxID_ANY, "New Password:"), 0, wxLEFT|wxRIGHT|wxTOP, 15);
    wxTextCtrl* newPass = new wxTextCtrl(&dlg, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    mainSizer->Add(newPass, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    mainSizer->Add(new wxStaticText(&dlg, wxID_ANY, "Confirm New Password:"), 0, wxLEFT|wxRIGHT|wxTOP, 15);
    wxTextCtrl* confirmPass = new wxTextCtrl(&dlg, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    mainSizer->Add(confirmPass, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    wxStaticText* errorText = new wxStaticText(&dlg, wxID_ANY, "");
    errorText->SetForegroundColour(*wxRED);
    mainSizer->Add(errorText, 0, wxLEFT|wxRIGHT|wxTOP, 15);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okBtn     = new wxButton(&dlg, wxID_ANY, "Change Password");
    wxButton* cancelBtn = new wxButton(&dlg, wxID_CANCEL, "Cancel");
    btnSizer->Add(okBtn,     0, wxRIGHT, 10);
    btnSizer->Add(cancelBtn, 0);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 15);

    dlg.SetSizer(mainSizer);
    mainSizer->Fit(&dlg);
    dlg.Layout();
    dlg.Centre(wxBOTH);

    okBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
        if (currentPass->IsEmpty() || newPass->IsEmpty() || confirmPass->IsEmpty()) {
            errorText->SetLabel("All fields are required.");
            dlg.Layout();
            return;
        }
        if (currentPass->GetValue()!=m_system->getConfig()->getPassword()) {
            errorText->SetLabel("Current Password Incorrect");
            currentPass->Clear();
            dlg.Layout();
            return;
        }
        if (newPass->GetValue() != confirmPass->GetValue()) {
            errorText->SetLabel("Passwords do not match.");
            confirmPass->Clear();
            dlg.Layout();
            return;
        }
        if (currentPass->GetValue() == newPass->GetValue()) {
            errorText->SetLabel("New password must be different.");
            dlg.Layout();
            return;
        }

        dlg.EndModal(wxID_OK);
    });

    if (dlg.ShowModal() == wxID_OK) {
        m_system->getConfig()->setPassword(string(newPass->GetValue()));
        wxMessageBox("Password changed successfully!", "Success", wxOK|wxICON_INFORMATION);
        changesMade = true;
    }
}

void Config_Panel::onChangeAlarm(wxCommandEvent &event) {
    wxDialog dlg(this, wxID_ANY, "Alarm Settings", wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(new wxStaticText(&dlg, wxID_ANY, "Alarm Sound:"), 0, wxLEFT|wxRIGHT|wxTOP, 15);
    wxArrayString sounds;
    sounds.Add("Beep");
    sounds.Add("Siren");
    sounds.Add("Buzzer");
    sounds.Add("Alert Tone");
    sounds.Add("Custom...");
    wxChoice* soundChoice = new wxChoice(&dlg, wxID_ANY, wxDefaultPosition, wxDefaultSize, sounds);
    soundChoice->SetSelection(0);
    mainSizer->Add(soundChoice, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    mainSizer->Add(new wxStaticText(&dlg, wxID_ANY, "Base Volume:"), 0, wxLEFT|wxRIGHT|wxTOP, 15);
    wxBoxSizer* volumeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSlider* volumeSlider = new wxSlider(&dlg, wxID_ANY, 50, 0, 100,
                                          wxDefaultPosition, wxDefaultSize,
                                          wxSL_HORIZONTAL);
    wxStaticText* volumeLabel = new wxStaticText(&dlg, wxID_ANY, "50%",
                                                  wxDefaultPosition, wxSize(35, -1));
    volumeSizer->Add(volumeSlider, 1, wxEXPAND|wxRIGHT, 5);
    volumeSizer->Add(volumeLabel, 0, wxALIGN_CENTER_VERTICAL);
    mainSizer->Add(volumeSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    volumeSlider->Bind(wxEVT_SLIDER, [&](wxCommandEvent&) {
        volumeLabel->SetLabel(wxString::Format("%d%%", volumeSlider->GetValue()));
    });

    mainSizer->Add(new wxStaticText(&dlg, wxID_ANY, "Max Duration (seconds):"), 0, wxLEFT|wxRIGHT|wxTOP, 15);
    wxSpinCtrl* durationSpin = new wxSpinCtrl(&dlg, wxID_ANY, "30",
                                               wxDefaultPosition, wxDefaultSize,
                                               wxSP_ARROW_KEYS, 5, 300, 30);
    mainSizer->Add(durationSpin, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);
    mainSizer->Add(new wxStaticText(&dlg, wxID_ANY, "Min: 5s     Max: 300s"),
                   0, wxLEFT|wxRIGHT, 15);

    wxStaticText* errorText = new wxStaticText(&dlg, wxID_ANY, "");
    errorText->SetForegroundColour(*wxRED);
    mainSizer->Add(errorText, 0, wxLEFT|wxRIGHT|wxTOP, 15);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okBtn     = new wxButton(&dlg, wxID_ANY, "Save");
    wxButton* cancelBtn = new wxButton(&dlg, wxID_CANCEL, "Cancel");
    btnSizer->Add(okBtn,     0, wxRIGHT, 10);
    btnSizer->Add(cancelBtn, 0);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 15);

    dlg.SetSizer(mainSizer);
    mainSizer->Fit(&dlg);
    dlg.SetMinSize(dlg.GetSize());
    dlg.Layout();
    dlg.Centre(wxBOTH);

    okBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
        dlg.EndModal(wxID_OK);
    });

    if (dlg.ShowModal() == wxID_OK) {
        int     sound    = soundChoice->GetSelection();
        int     volume   = volumeSlider->GetValue();
        int     duration = durationSpin->GetValue();

        // m_system->SetAlarmSound(sound);
        // m_system->SetAlarmVolume(volume);
        // m_system->SetAlarmDuration(duration);

        wxMessageBox("Alarm settings saved!", "Success", wxOK|wxICON_INFORMATION);
        changesMade=true;
    }
}

void Config_Panel::onChangeClipLen(wxCommandEvent &event) {
    wxDialog dlg(this, wxID_ANY, "Photo/Video Settings", wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* enableSizer = new wxStaticBoxSizer(wxVERTICAL, &dlg, "Capture Mode");
    wxCheckBox* enablePhoto = new wxCheckBox(&dlg, wxID_ANY, "Enable Photo/Video Capture");
    enablePhoto->SetValue(true);
    enableSizer->Add(enablePhoto,0, wxALL, 8);
    mainSizer->Add(enableSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    wxStaticBoxSizer* clipSizer = new wxStaticBoxSizer(wxVERTICAL, &dlg, "Recording Clip Length");
    wxBoxSizer* clipRowSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSlider* clipSlider = new wxSlider(&dlg, wxID_ANY, 30, 5, 120,
                                         wxDefaultPosition, wxDefaultSize,
                                         wxSL_HORIZONTAL);
    wxStaticText* clipLabel = new wxStaticText(&dlg, wxID_ANY, "30s",
                                                wxDefaultPosition, wxSize(40, -1));
    clipRowSizer->Add(clipSlider, 1, wxEXPAND|wxRIGHT, 5);
    clipRowSizer->Add(clipLabel,  0, wxALIGN_CENTER_VERTICAL);
    clipSizer->Add(clipRowSizer, 0, wxEXPAND|wxALL, 8);
    clipSizer->Add(new wxStaticText(&dlg, wxID_ANY, "Min: 5s     Max: 120s"),
                   0, wxLEFT|wxBOTTOM, 8);
    mainSizer->Add(clipSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    clipSlider->Bind(wxEVT_SLIDER, [&](wxCommandEvent&) {
        clipLabel->SetLabel(wxString::Format("%ds", clipSlider->GetValue()));
    });

    wxStaticBoxSizer* photoSizer = new wxStaticBoxSizer(wxVERTICAL, &dlg, "Photos Per Capture");
    wxBoxSizer* photoRowSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSpinCtrl* photoSpin = new wxSpinCtrl(&dlg, wxID_ANY, "3",
                                            wxDefaultPosition, wxDefaultSize,
                                            wxSP_ARROW_KEYS, 1, 20, 3);
    wxStaticText* photoHint = new wxStaticText(&dlg, wxID_ANY, "photos per trigger event");
    photoRowSizer->Add(photoSpin, 0, wxRIGHT, 10);
    photoRowSizer->Add(photoHint, 0, wxALIGN_CENTER_VERTICAL);
    photoSizer->Add(photoRowSizer, 0, wxALL, 8);
    mainSizer->Add(photoSizer, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 15);

    enablePhoto->Bind(wxEVT_CHECKBOX, [&](wxCommandEvent&) {
        photoSpin->Enable(enablePhoto->GetValue());
        photoHint->Enable(enablePhoto->GetValue());
    });

    wxStaticText* errorText = new wxStaticText(&dlg, wxID_ANY, "");
    errorText->SetForegroundColour(*wxRED);
    mainSizer->Add(errorText, 0, wxLEFT|wxRIGHT|wxTOP, 15);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okBtn     = new wxButton(&dlg, wxID_ANY, "Save");
    wxButton* cancelBtn = new wxButton(&dlg, wxID_CANCEL, "Cancel");
    btnSizer->Add(okBtn,     0, wxRIGHT, 10);
    btnSizer->Add(cancelBtn, 0);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 15);

    enablePhoto->SetValue(m_system->getConfig()->getCaptureMode());
    clipSlider->SetValue(m_system->getConfig()->getSeconds());
    clipLabel->SetLabel(wxString::Format("%ds", m_system->getConfig()->getSeconds()));
    photoSpin->SetValue(m_system->getConfig()->getPhotosPer());

    // Reflect initial enabled state
    photoSpin->Enable(m_system->getConfig()->getCaptureMode());

    dlg.SetSizer(mainSizer);
    mainSizer->Fit(&dlg);
    dlg.SetMinSize(dlg.GetSize());
    dlg.Layout();
    dlg.Centre(wxBOTH);

    okBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
        dlg.EndModal(wxID_OK);
    });

    if (dlg.ShowModal() == wxID_OK) {
        bool photoEnabled     = enablePhoto->GetValue();
        int  clipLength       = clipSlider->GetValue();
        int  photosPerCapture = photoSpin->GetValue();

        m_system->getConfig()->setCaptureMode(photoEnabled);
        m_system->getConfig()->setSeconds(clipLength);
        m_system->getConfig()->setPhotosPer(photosPerCapture);

        wxMessageBox("Photo/Video settings saved!", "Success", wxOK|wxICON_INFORMATION);
        changesMade = true;
    }
}

void Config_Panel::onChangeMotion(wxCommandEvent &event) {
    wxDialog dlg(this, wxID_ANY, "Motion Sensitivity", wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* sensitivitySizer = new wxStaticBoxSizer(wxVERTICAL, &dlg, "Motion Sensitivity");

    wxBoxSizer* sliderRowSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* lowLabel  = new wxStaticText(&dlg, wxID_ANY, "Low");
    wxSlider*     slider    = new wxSlider(&dlg, wxID_ANY, 50, 0, 100,
                                           wxDefaultPosition, wxSize(200, -1),
                                           wxSL_HORIZONTAL);
    wxStaticText* highLabel = new wxStaticText(&dlg, wxID_ANY, "High");
    sliderRowSizer->Add(lowLabel,  0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 8);
    sliderRowSizer->Add(slider,    1, wxEXPAND);
    sliderRowSizer->Add(highLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 8);
    sensitivitySizer->Add(sliderRowSizer, 0, wxEXPAND|wxALL, 8);

    wxStaticText* valueLabel = new wxStaticText(&dlg, wxID_ANY, "50%",
                                                 wxDefaultPosition, wxDefaultSize,
                                                 wxALIGN_CENTER_HORIZONTAL);
    sensitivitySizer->Add(valueLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 8);

    mainSizer->Add(sensitivitySizer, 0, wxEXPAND|wxALL, 15);

    slider->Bind(wxEVT_SLIDER, [&](wxCommandEvent&) {
        valueLabel->SetLabel(wxString::Format("%d%%", slider->GetValue()));
    });

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okBtn     = new wxButton(&dlg, wxID_ANY, "Save");
    wxButton* cancelBtn = new wxButton(&dlg, wxID_CANCEL, "Cancel");
    btnSizer->Add(okBtn,     0, wxRIGHT, 10);
    btnSizer->Add(cancelBtn, 0);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 15);

    dlg.SetSizer(mainSizer);
    mainSizer->Fit(&dlg);
    dlg.SetMinSize(dlg.GetSize());
    dlg.Layout();
    dlg.Centre(wxBOTH);

    slider->SetValue(m_system->getConfig()->getSensitivity());
    valueLabel->SetLabel(wxString::Format("%d%%", m_system->getConfig()->getSensitivity()));

    okBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
        dlg.EndModal(wxID_OK);
    });

    if (dlg.ShowModal() == wxID_OK) {
        m_system->getConfig()->setSensitivity(slider->GetValue());
        wxMessageBox("Motion sensitivity saved!", "Success", wxOK|wxICON_INFORMATION);
        changesMade = true;
    }
}

void Config_Panel::onSave(wxCommandEvent &event) {
    if (!m_system->getConfig()->writeToFile()) {
        wxMessageBox("Settings Failed to Save", "Failure", wxOK|wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Settings Saved!", "Success", wxOK|wxICON_INFORMATION);
        changesSaved = true;
    }
}

void Config_Panel::onHomeButton(wxCommandEvent &event) {
    if (changesMade && !changesSaved) {
        int confirm = wxMessageBox("Are you Sure you want to leave without saving your changes", "Warning", wxYES|wxNO|wxICON_WARNING);
        if (confirm == wxID_YES) {
            m_ui->SwitchPage(UI::Home_ID);
        }
    }
    else {
        m_ui->SwitchPage(UI::Home_ID);
    }
}


Config_Panel::~Config_Panel() {}
