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
    //Sets the register inside the PCA9685 with address 'Register' to 'Value'
    uint8_t setReg(uint8_t Register, uint8_t Value);
    //Returns the value of the register inside the PCA9685 with address 'Register'
    uint8_t getReg(uint8_t Register);

    void setFrequency();

    //Sets the on-cycle duration of the PWM to 'Value'
    void setPWM_ON(uint16_t Value, uint8_t PWM);
    //Sets the off-cycle duration of the PWM to 'Value'
    void setPWM_OFF(uint16_t Value, uint8_t PWM);

    //Returns the duration of the PWM's on-cycle
    uint16_t getPWM_ON(uint8_t PWM);
    //Returns the duration of the PWM's off-cycle
    uint16_t getPWM_OFF(uint8_t PWM);
};

#endif