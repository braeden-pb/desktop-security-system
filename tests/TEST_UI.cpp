//
// Created by evan on 2026-03-06.
//

#include  <wx/uiaction.h>
#include <wx/wx.h>
#include "../Login_Panel.h"
#include "../SecuritySystem.h"
#include "../UI.h"
#include <wx/window.h>

void TestCorrectPass() {
    SecuritySystem* system = new SecuritySystem();
    wxFrame* dummyFrame = new wxFrame(nullptr, wxID_ANY, "Test Frame");
    UI* testUI = new UI(system);
    wxUIActionSimulator sim;
    Login_Panel* panel = testUI->getLoginPanel();
    panel->GetPasswordCtrl()->SetFocus();
    sim.Text("1234");


    wxWindow* loginBtn = panel->GetLoginButton();
    int x,y;
    loginBtn->GetScreenPosition(&x,&y);

    sim.MouseMove(pos.x + (loginBtn->GetSize().x / 2),
                      pos.y + (loginBtn->GetSize().y / 2));
    sim.MouseClick(wxMOUSE_BTN_LEFT);

    wxYield();

    assert(panel->isLoginSuccessful()==true);
}

void TestIncorrectPass() {
    SecuritySystem* system = new SecuritySystem();
    wxFrame* dummyFrame = new wxFrame(nullptr, wxID_ANY, "Test Frame");
    UI* testUI = new UI(system);
    Login_Panel* panel = new Login_Panel(dummyFrame,system,testUI);
    wxUIActionSimulator sim;
    panel->GetPasswordCtrl()->SetFocus();
    sim.Text("1235");


    wxWindow* loginBtn = panel->GetLoginButton();
    int x,y;
    wxPoint pos = loginBtn->GetScreenPosition();

    sim.MouseMove(pos.x + (loginBtn->GetSize().x / 2),
                      pos.y + (loginBtn->GetSize().y / 2));
    sim.MouseClick(wxMOUSE_BTN_LEFT);

    wxYield();

    assert(panel->isLoginSuccessful()==false);



}

int main(int argc, char *argv[]) {
    TestCorrectPass();
    TestIncorrectPass();
}

