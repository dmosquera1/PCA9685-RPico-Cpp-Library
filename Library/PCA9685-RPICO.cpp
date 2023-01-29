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

void PCA9685::setPWM_ON(uint16_t Value, uint8_t PWM) {
    uint8_t msg[2] = {(uint8_t)Value, (uint8_t)(Value >> 8)};
    uint8_t PWM_INDEX = PWM*4 + 6;
    setReg(PWM_INDEX, msg[0]);
    setReg(PWM_INDEX + 1, msg[1]);
}

void PCA9685::setPWM_OFF(uint16_t Value, uint8_t PWM) {
    uint8_t msg[2] = {(uint8_t)Value, (uint8_t)(Value >> 8)};
    uint8_t PWM_INDEX = PWM*4 + 6;
    setReg(PWM_INDEX + 2, msg[0]);
    setReg(PWM_INDEX + 3, msg[1]);
}