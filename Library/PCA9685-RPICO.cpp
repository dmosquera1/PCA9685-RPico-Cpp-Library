#include "PCA9685-RPICO.hpp"
#include "pico/stdlib.h"

uint8_t PCA9685::setReg(uint8_t Register, uint8_t Value) {
    uint8_t msg[2] = {Register, Value};
    return i2c_write_blocking(I2C_Pins, I2C_Address, msg, 2, false);
}

uint8_t PCA9685::readReg(uint8_t Register) {
    uint8_t temp;
    i2c_write_blocking(I2C_Pins, I2C_Address, &Register, 1, false);
    i2c_read_blocking(I2C_Pins, I2C_Address, &temp, 1, false);
    return temp;
}

uint8_t PCA9685::setFrequency() {
    
}