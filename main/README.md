/* 
* MAX31855 Thermocouple Temperature Software - for type K
* Author: BBQBailey, see //github.com/bbqbailey for updates
* Date: May 13, 2018
*
* Rev: 1.0.1
*   Initial release May 15 2018
*       - changed error and fault to uint8_t.
*       - added detection of error and fault to rawData
*       - Removed non-used negativeCalcs function
*       - Added 'std::string* getName(void)' function  this is to 
*         facilitate the use of multiple sensors.  Will probably 
*         be dropped when implemented in full Device/Sensor design,
*         as it will reside there.  But for now .... it's in there!
*       - modified Thermocouple constructors to add std::string name
*       - added new private data element 'std::string* name'
*
* Rev: 1.0
*   Initial Release May 13 2018
*
* License: GNU Lesser General Public License v3.0
*   - Please refer to LICENSE file in this repository for 
*     additional details.
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
