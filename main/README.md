/* 
* MAX31855 Thermocouple Temperature Software - for type K
* Author: BBQBailey, see //github.com/bbqbailey for updates
* Date: May 14, 2018
*
* Rev: 1.0
*
*  License: GNU Lesser General Public License v3.0
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
* This release changes:
*   - Add calculations for negative temperatures
* Next Release:
*   - Use interrupt callbacks instead of vTaskDelay(), which is a blocking call.
*   - Add web page host
*   - Add MQTT broker communications

*/
