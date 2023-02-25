//C++ Standard Libraries
#include <stdio.h>
#include <memory>
#include <string>
#include <cstring>

//Pico SDK Libraries
#include "pico/stdlib.h"
//#include <pico/binary_info/code.h>
//#include "pico/multicore.h"

//Project Libraries
#include "../Library/PCA9685-RPICO.hpp"

#define RotaryEncoderA 6
#define RotaryEncoderB 7
#define RotaryEncoderSW 8

PCA9685 PCA1(0x40, i2c0);
double PWM_VALUE_1 = 1;
double PWM_VALUE_2 = 0.5;

//Diagnostic Timer Callback
volatile bool timer_fired = false;
bool alarm_callback(struct repeating_timer *t) {
    printf("LED0_Off: %d    LED1_Off: %d\n", PCA1.getPWM_OFF(0), PCA1.getPWM_OFF(1));
    printf("PWM_1:    %f       PWM_0: %f\n", PWM_VALUE_2, PWM_VALUE_1);
    if (gpio_get(25)) {
        gpio_put(25, false);
    } else {
        gpio_put(25,true);
    }
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

    //PCA9685 Initialization
    
    uint8_t Sweep_Up1 = 1;

    PCA1.setReg(0x00, 0xA0);
    PCA1.setReg(0x01, 0x04);

    //Setting Up Diagnostic Timer Callback
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, alarm_callback, NULL, &timer);

    PCA1.setFrequency(50);

    PCA1.setPWM_Duty_Cycle(0.75, 0);
    PCA1.setPWM_Duty_Cycle(0.05, 1);

    PCA1.setPWM_Duty_Cycle(0.95, 2);
    PCA1.setPWM_Duty_Cycle(0.9, 3);

    gpio_set_dir(25, true);

    while (1) {
        //Sweeping LEDs
        if (PWM_VALUE_1 >= 1) Sweep_Up1 = 0;
        if (PWM_VALUE_1 <= 0.5) Sweep_Up1 = 1;
        if (Sweep_Up1) {
            PWM_VALUE_1 += 0.005;
            PWM_VALUE_2 -= 0.005;
        }
        if (!Sweep_Up1) {
            PWM_VALUE_1 -= 0.005;
            PWM_VALUE_2 += 0.005;
        }
        PCA1.setPWM_Duty_Cycle(PWM_VALUE_1, 0);
        PCA1.setPWM_Duty_Cycle(PWM_VALUE_2, 1);
        sleep_ms(20);
    }
}