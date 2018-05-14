#include "Thermocouple.h"

Thermocouple::Thermocouple(void) {
    this->cs = gpio_num_t(CS);
    this->clk = gpio_num_t(CLK);
    this->data = gpio_num_t(DATA);

    this->init();
}

Thermocouple::Thermocouple(gpio_num_t cs, gpio_num_t clk, gpio_num_t data) {
    this->cs   = cs;
    this->clk  = clk;
    this->data = data;

    this->init();
}

void Thermocouple::init(void) {
    //assign GPI functions
    gpio_set_direction(gpio_num_t(CS), GPIO_MODE_OUTPUT);
    gpio_set_direction(gpio_num_t(CLK), GPIO_MODE_OUTPUT);
    gpio_set_direction(gpio_num_t(DATA), GPIO_MODE_INPUT);
}

void Thermocouple::getData(void) {
    readCounter++;
    gpio_set_level(gpio_num_t(cs), 0);  //select MAX31855
    //get the 32 bits from MAX31855
    //   generate clock pulse high, then read data bit, then clock pulse low
    std::cout <<"getData====\n";
    if(DEBUG) {
        std::cout << "Read Counter: " << readCounter << "\n";
        std::cout << "Binary data received from MAX31855: ";
    };

    //get 32 bits of data from MAX31855
    for(int i=0;i<31;i++) {
        gpio_set_level(gpio_num_t(clk), 1);  //clock pulse going high - start pulse
        valueBit = gpio_get_level(gpio_num_t(data));  //get bit
        rawData <<= 1;
        rawData += 0x01 & valueBit;  
        gpio_set_level(gpio_num_t(clk), 0);  //clock pulse going low - end pulse
        if(DEBUG)
            std::cout << valueBit;
    }
    gpio_set_level(gpio_num_t(cs), 1); //de-select MAX31855


    if(DEBUG) {
        std::cout << "###TESTING!###\n";
        rawData = 0xFFFC0000;
    }

    internData = (rawData & 0x7FF0) >> 4; //dump D16 'fault bit'
    thermData = (rawData >> 18);
    tempIntC = float(internData) * 0.0625;
    tempIntF = tempIntC * 1.8 + 32.0;
    tempC   = (float(thermData) * .25) + tempIntC;
    tempF   = tempC * 1.8 + 32.0;

    if(DEBUG) {
        printf("tempIntC: %6.1f \n", tempIntC);
        printf("tempIntF: %6.1f \n", tempIntF);
        printf("tempC: %6.1f \n", tempC);
        printf("TempF: %6.1f \n", tempF);
        std::cout << "\n";
    }

}


float   Thermocouple::getTempF(void) {
    getData();
    return tempF;
}

float Thermocouple::getTempC(void) {
    getData();
    return tempC;
}

void Thermocouple::printData(void) {
        std::cout << "\n";
        std::cout << "Read Counter: " << readCounter << "\n";
        printf("error: ox%X\n", error);  //will not be zero if error returned from chip
        if(DEBUG) {
            std::bitset<32>  x(rawData);
            std::cout << "Binary: " << x << "\n";
            printf("rawData: 0x%X\n", rawData);
            printf("internData: 0x%X\n", internData);
            printf("thermData: 0x%X\n", thermData);
        }   
        printf("tempIntC: %6.1f \n", tempIntC);
        printf("tempIntF: %6.1f \n", tempIntF);
        printf("tempC: %6.1f \n", tempC);
        printf("TempF: %6.1f \n", tempF);
        printf("NOTE: NEED TO ADD FAULT DETECTION FOR D16\n");
        std::cout << "\n";
}


