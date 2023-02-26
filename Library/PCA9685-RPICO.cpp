#include "PCA9685-RPICO.hpp"
#include "pico/stdlib.h"

uint8_t PCA9685::setReg(uint8_t Register, uint8_t Value) {
    uint8_t msg[2] = {Register, Value};
    return i2c_write_blocking(I2C_Pins, I2C_Address, msg, 2, false);
}

uint8_t PCA9685::getReg(uint8_t Register) {
    uint8_t temp;
    i2c_write_blocking(I2C_Pins, I2C_Address, &Register, 1, false);
    i2c_read_blocking(I2C_Pins, I2C_Address, &temp, 1, false);
    return temp;
}

void PCA9685::setFrequency(double Frequency) {
    //Only attempt to set frequency if given value is between 24 and 1526
    if (Frequency >= 24 && Frequency <= 1526) {
        uint8_t PRE_SCALE_VALUE = round(25000000.0/(4096.0*Frequency)) - 1;
        //Change only the 'sleep' bit in the MODE1 register to 1 to set the oscillator to sleep
        uint8_t currentMODE1 = getReg(0x00);
        setReg(0x00, currentMODE1|0x10);
        //Change the frequency of the PWM channels using the previously calculated PRE_SCALE_VALUE
        setReg(0xFE, PRE_SCALE_VALUE);
        while (!(getReg(0x00) | 0x80)) {
            sleep_ms(5);
        }
        //Wait 500us for oscillator to stabilize before setting RESTART bit to 1
        sleep_us(500);
        setReg(0x00, currentMODE1|0x80);
    }
}

void PCA9685::PCA_Start() {
    //Set control register 1 to 10100000
    setReg(0x00, 0xA0);
    //Set control register 2 to 00000100
    setReg(0x01, 0x04);
}


void PCA9685::setPWM_ON(uint16_t Value, uint8_t PWM) {
    //Make an array of two valid bytes out of the 16-bit 'Value' parameter
    uint8_t msg[2] = {(uint8_t)(Value), (uint8_t)(Value >> 8)};
    //Translate the PWM parameter into the corresponding PCA9685 register's address
    uint8_t PWM_INDEX = PWM*4 + 6;
    setReg(PWM_INDEX, msg[0]);
    //Set the "Always On" bit to 0 while writing the high byte
    setReg(PWM_INDEX + 1, msg[1] | 0x00);
}

void PCA9685::setPWM_OFF(uint16_t Value, uint8_t PWM) {
    //Make an array of two valid bytes out of the 16-bit 'Value' parameter
    uint8_t msg[2] = {(uint8_t)(Value), (uint8_t)(Value >> 8)};
    uint8_t PWM_INDEX = PWM*4 + 6;
    //Translate the PWM parameter into the corresponding PCA9685 register's address
    setReg(PWM_INDEX + 2, msg[0]);
    setReg(PWM_INDEX + 3, msg[1] & 0x0F);
}

uint16_t PCA9685::getPWM_ON(uint8_t PWM) {
    uint16_t lowByte, highByte;
    //Translate the PWM parameter into the corresponding PCA9685 register's address
    uint8_t PWM_INDEX = (PWM*4 + 6);
    lowByte = getReg(PWM_INDEX);
    //Mask the top 4 bits of LEDX_ON_H to 0 and cast remainder to 16-bit integer, then shift to occupy upper byte
    highByte = ((uint16_t)getReg(PWM_INDEX + 1) & 0x0F) << 8; 
    return (highByte | lowByte);
}

uint16_t PCA9685::getPWM_OFF(uint8_t PWM) {
    uint16_t lowByte, highByte;
    //Translate the PWM parameter into the corresponding PCA9685 register's address
    uint8_t PWM_INDEX = (PWM*4 + 6);
    lowByte = getReg(PWM_INDEX + 2);
    //Mask the top 4 bits of LEDX_ON_H to 0 and cast remainder to 16-bit integer, then shift to occupy upper byte
    highByte = ((uint16_t)getReg(PWM_INDEX + 3) & 0x0F) << 8; 
    return (highByte | lowByte);
}

void PCA9685::setPWM_Duty_Cycle(double Value, uint8_t PWM) {
    uint16_t countValue = round(4096.0*(Value)) - 1;
    setPWM_ON(0, PWM);
    setPWM_OFF(countValue, PWM);
}