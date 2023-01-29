#ifndef PCA9685_PICO_H
#define PCA9685_PICO_H
#include "pico/stdlib.h"
#include "hardware/i2c.h"

class PCA9685 {
    private:
        int8_t I2C_Address;
        i2c_inst_t* I2C_Pins;
    public:
        PCA9685(uint8_t address, i2c_inst_t* i2c) {
            I2C_Address = address;
            I2C_Pins = i2c;
        }
    uint8_t setReg(uint8_t Register, uint8_t Value);
    uint8_t readReg(uint8_t Register);
    uint8_t setFrequency();
};

#endif