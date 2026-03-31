#include "Alert.h"
#include "SecuritySystem.h"
#include <iostream>

class TestUI : public IAlertCallback {
public:
    void onAlert(const std::string& email, const std::string& time) override {
        std::cout << "UI received alert: " << email << " at " << time << std::endl;
    }
};

int main() {
    SecuritySystem system;
    Event event("2026-03-06 15:00");
    Alert alert(event, "test@email.com", system);

    // test WITHOUT callback
    std::cout << "Testing sendAlert() without UI..." << std::endl;
    alert.sendAlert();

    // test WITH callback
    TestUI ui;
    alert.setUICallback(&ui);
    std::cout << "Testing sendAlert() with UI..." << std::endl;
    alert.sendAlert();

    std::cout << "Testing getEvent()" << std::endl;
    Event e = alert.getEvent();
    std::cout << e.getTime() << std::endl;

    std::cout << "Testing discard()" << std::endl;
    alert.discard();
}
