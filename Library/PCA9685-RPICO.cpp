#include "PCA9685-RPICO.hpp"
#include "pico/stdlib.h"

uint8_t PCA9685::setReg(uint8_t* src) {
    return i2c_write_blocking(I2C_Pins, I2C_Address, src, 2, false);
}

uint8_t PCA9685::readReg(uint8_t reg) {
    uint8_t temp;
    i2c_write_blocking(I2C_Pins, I2C_Address, &reg, 1, false);
    i2c_read_blocking(I2C_Pins, I2C_Address, &temp, 1, false);
    return temp;
}