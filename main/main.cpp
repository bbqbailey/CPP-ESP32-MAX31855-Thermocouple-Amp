/* 
* MAX31855 Thermocouple Temperature Software - for type K
* Author: BBQBailey, see //github.com/bbqbailey for updates
* Date: May 13, 2018
* Rev: 1.0
* Purpose: Software for MAX31855 on ESP32
*   I was not able to use the Espressif SPI code to work
*   correctly with my MAX31855 thermocouple amplifier for 
*   type K thermocouples, so I wrote this code.  
*   It works correctly for me without any vTaskDelay() calls
*
*   It does not use the hardware SPI capabilities. 
*   Instead, it generates its own clock pulses, and reads the bit 
*   responses.
*
*   It works correctly for me without any vTaskDelay() calls between
*   CLK, CS, MISO signals.
*
* Next Release:
*   - Add calculations for negative temperatures
*   - Use interrupt callbacks instead of vTaskDelay(), which is a blocking call.
*   - Add web page host
*   - Add MQTT broker communications

*/

#include <iostream>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h> //for vTaskDelay();
#include <freertos/task.h>     //for vTaskDelay();

//NOTE: the following pins all have an LED attached to them.  Low causes light
#define CS   16  //output - CHIP SELECT
#define CLK  17  //output - CLOCK
#define DATA 18  //input  - DATA AKA MISO

extern "C" {
    void app_main(void);
}

void app_main(void) {
    int valueBit = 0;
    uint32_t rawData = 0;
    uint32_t  error = 0;
    uint8_t  signInt = 0;
    uint8_t  signC = 0;

    float tempIntC = 0.0;
    float tempIntF = 0.0;
    float tempC = 0.0;
    float tempF = 0.0;

    //assign GPI functions
    gpio_set_direction(gpio_num_t(CS), GPIO_MODE_OUTPUT);
    gpio_set_direction(gpio_num_t(CLK), GPIO_MODE_OUTPUT);
    gpio_set_direction(gpio_num_t(DATA), GPIO_MODE_INPUT);

    //start doing it!
    gpio_set_level(gpio_num_t(CS), 1);
    gpio_set_level(gpio_num_t(CLK), 0);

    //create our own clock pulses, then read data bits
    while(1) {
        gpio_set_level(gpio_num_t(CS), 0);  //enable chip
//        vTaskDelay(10);  //uncomment if you are having problems to give chip time to synch with clock

        //get the 32 bits from MAX31855
        //   generate clock pulse high, then read data bit, then clock pulse low
        std::cout << "Binary data received from MAX31855: ";
        for(int i=0;i<31;i++) {
            gpio_set_level(gpio_num_t(CLK), 1);  //clock pulse going high - start pulse
//            vTaskDelay(10);  //uncomment if synch problem
            valueBit = gpio_get_level(gpio_num_t(DATA));  //get bit
            rawData <<= 1;
            rawData += 0x01 & valueBit;  
//            vTaskDelay(10);  //uncomment if synch problem
            gpio_set_level(gpio_num_t(CLK), 0);  //clock pulse going low - end pulse
//            vTaskDelay(10);  //uncomment if synch problem
            std::cout << valueBit;
        }
        gpio_set_level(gpio_num_t(CS), 1);
        std::cout << "\n";

        printf("rawData: 0x%x\n", rawData);
        error = rawData & 0x7;
        if(error) {
            printf("-----Error reported by chip.  Value is: 0x%x\n", error);
            printf("     Following values are probably bad.\n");
        }

        signInt = (rawData & 0x8000) >> 4;
        signC   = (rawData & 0x80000000) >> 31;


        if((signInt == 0) && (signC == 0)) {

            tempIntC = float((rawData >> 4) & 0x7FF) * 0.0625;
            tempIntF = tempIntC * 1.8 + 32.0;
            tempC   = (float((rawData >> 18) & 0x7FFC) * .25 + tempIntC);
            tempF   = tempC * 1.8 + 32.0;
            printf("error: ox%x\n", error);  //will not be zero if error returned from chip
            printf("signInt: ox%x\n", signInt);
            printf("signC: ox%x\n", signC);
            printf("tempIntC: %6.2f \n", tempIntC);
            printf("tempIntF: %6.2f \n", tempIntF);
            printf("tempC: %6.2f \n", tempC);
            printf("TempF: %6.2f \n", tempF);
        }   else  {
            std::cout << "-----negative temperatures not yet implemented.-----\n";
        }

        vTaskDelay(15000);  //or what ever you want.
    }
}
