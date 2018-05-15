/* 
* MAX31855 Thermocouple Temperature Software - for type K
* Author: BBQBailey, see //github.com/bbqbailey for updates
* Date: May 13, 2018
* Rev: 1.0
* License: GNU Lesser General Public License v3.0
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
#include <bitset>

//NOTE: the following pins all have an LED attached to them.  Low causes light
#define CS   16  //output - CHIP SELECT
#define CLK  17  //output - CLOCK
#define DATA 18  //input  - DATA AKA MISO

#define DEBUG 0

class Thermocouple {
    public:
        Thermocouple(void);
        Thermocouple(gpio_num_t cs, gpio_num_t clk, gpio_num_t data);

        float getTempC(void);
        float getTempF(void);
        void printData(void);
        void negativeValue(void);

    private:
        gpio_num_t cs;
        gpio_num_t clk;
        gpio_num_t data;
        unsigned long int readCounter = 0;

        int valueBit = 0;
        int32_t rawData = 0;
        int16_t thermData = 0;
        int16_t rjData = 0;
        uint32_t  error = 0;
        uint8_t fault=0;

        float tempRJC = 0.0;
        float tempRJF = 0.0;
        float tempThermC = 0.0;
        float tempThermF = 0.0;
        float tempC = 0.0;
        float tempF = 0.0;

        void init(void);
        void getData(void);
};

