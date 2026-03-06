#include "Alert.h"
#include "Event.h"
#include "SecuritySystem.h"
#include <iostream>

int main() {

    SecuritySystem system;
    Event event("2026-03-06 15:00");

    Alert alert(event, "test@email.com", system);

    std::cout << "Testing sendAlert()" << std::endl;
    alert.sendAlert();

    std::cout << "Testing getEvent()" << std::endl;
    Event e = alert.getEvent();
    std::cout << e.getTime() << std::endl;

    std::cout << "Testing discard()" << std::endl;
    alert.discard();

}