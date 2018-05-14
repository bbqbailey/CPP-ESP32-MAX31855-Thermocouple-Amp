#include "Thermocouple.h"

extern "C" {
    void app_main(void);
}

void app_main(void) {
    Thermocouple thermocouple = Thermocouple();
    while(1) {
        thermocouple.getTempF();
        thermocouple.printData();
        vTaskDelay(5000);
    }
}