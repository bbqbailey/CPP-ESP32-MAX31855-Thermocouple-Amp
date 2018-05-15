#include "Thermocouple.h"

extern "C" {
    void app_main(void);
}

void app_main(void) {
    std::string name = "Combustion Chamber";

    Thermocouple thermocouple = Thermocouple(&name);
    while(1) {
        thermocouple.getTempF();
        thermocouple.printData();
        vTaskDelay(5000);
    }
}