#ifndef PCA9685_PICO_H
#define PCA9685_PICO_H
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>

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

    //Sets the frequency of all PWMs to 'Frequency' as long as 'Frequency' is between 24 and 1526
    void setFrequency(double Frequency);

    //Sets the on-cycle duration of the PWM to 'Value'
    void setPWM_ON(uint16_t Value, uint8_t PWM);
    //Sets the off-cycle duration of the PWM to 'Value'
    void setPWM_OFF(uint16_t Value, uint8_t PWM);

    //Returns the duration of the PWM's on-cycle
    uint16_t getPWM_ON(uint8_t PWM);
    //Returns the duration of the PWM's off-cycle
    uint16_t getPWM_OFF(uint8_t PWM);

    //Sets the duty cycle of the PWM (Value is a number between 0 and 1)
    void setPWM_Duty_Cycle(double Value, uint8_t PWM);
};

#endif