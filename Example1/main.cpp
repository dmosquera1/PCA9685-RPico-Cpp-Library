//C++ Standard Libraries
#include <stdio.h>
#include <memory>
#include <string>
#include <cstring>

//Pico SDK Libraries
#include "pico/stdlib.h"
//#include <pico/binary_info/code.h>
//#include "pico/multicore.h"

//Supplemental Libraries
#include "../Supplemental_Libraries/RotaryEncoderClass/RotaryEncoder.hpp"

//Project Libraries
#include "../Library/PCA9685-RPICO.hpp"

#define RotaryEncoderA 6
#define RotaryEncoderB 7
#define RotaryEncoderSW 8

PCA9685 PCA1(0x40, i2c0);
uint16_t PWM_VALUE_1 = 0, PWM_VALUE_2 = 0;

//Diagnostic Timer Callback
volatile bool timer_fired = false;
bool alarm_callback(struct repeating_timer *t) {
    printf("LED0_On: %d      LED0_Off: %d\n", PCA1.getPWM_ON(0), PCA1.getPWM_OFF(0));
    printf("PWM_VALUE_1 = %d    PWM_VALUE_2 = %d\n", PWM_VALUE_1, PWM_VALUE_2);
    return true;
}

int main() {

    //I2C Initialization
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
    

    //Rotary Encoder Initialization
    gpio_set_dir(RotaryEncoderA, 0);
    gpio_set_dir(RotaryEncoderB, 0);
    gpio_set_dir(RotaryEncoderSW, 0);
    gpio_pull_up(RotaryEncoderA);
    gpio_pull_up(RotaryEncoderB);
    gpio_pull_up(RotaryEncoderSW);

    int8_t Increment = 0;
    RotaryEncoder r1(RotaryEncoderA, RotaryEncoderB, RotaryEncoderSW);


    //PCA9785 Initialization
    
    uint8_t Sweep_Up1 = 1, R1_SW_Past = 0;

    PCA1.setReg(0x00, 0xA0);
    PCA1.setReg(0x01, 0x04);

    //Setting Up Diagnostic Timer Callback
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, alarm_callback, NULL, &timer);

    PCA1.setPWM_ON(0, 0);
    PCA1.setPWM_ON(0, 1);
    //PCA1.setReg(0x06, 0xFF);
    //PCA1.setReg(0x07, 0x0F);
    
    PCA1.setPWM_OFF(1, 0);
    PCA1.setPWM_OFF(4095, 1);

    while (1) {
        if (Sweep_Up1) PWM_VALUE_1 += 8;
        if (PWM_VALUE_1 >= 4095) Sweep_Up1 = 0;
        if (!Sweep_Up1) PWM_VALUE_1 -= 8;
        if (PWM_VALUE_1 <= 0) Sweep_Up1 = 1;


        PCA1.setPWM_OFF(PWM_VALUE_1, 0);
        PCA1.setPWM_ON(PWM_VALUE_1, 1);
        sleep_ms(5);
    }
}