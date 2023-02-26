//C++ Standard Libraries
#include <stdio.h>
#include <memory>
#include <string>
#include <cstring>

//Pico SDK Libraries
#include "pico/stdlib.h"

//Project Libraries
#include "../Library/PCA9685-RPICO.hpp"

#define Onboard_LED 25

PCA9685 PCA1(0x40, i2c0);
double PWM_VALUE_1 = 1;
double PWM_VALUE_2 = 0.5;

//Diagnostic Timer Callback
volatile bool timer_fired = false;
bool alarm_callback(struct repeating_timer *t) {
    printf("LED0_Off: %d    LED1_Off: %d\n", PCA1.getPWM_OFF(0), PCA1.getPWM_OFF(1));
    printf("PWM_1:    %f       PWM_0: %f\n", PWM_VALUE_2, PWM_VALUE_1);
    if (gpio_get(Onboard_LED)) {
        gpio_put(Onboard_LED, false);
    } else {
        gpio_put(Onboard_LED,true);
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

    uint8_t Sweep_Up1 = 1;

    //Blinking LED on Pico
    gpio_init(Onboard_LED);
    gpio_set_dir(Onboard_LED, true);

    //Setting Up Diagnostic Timer Callback
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, alarm_callback, NULL, &timer);


    //PCA9685 Initialization
    PCA1.PCA_Start();
    PCA1.setFrequency(50);

    while (1) {
        //Sweeping PWM Values
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

        //Assigning PWM Values to PWMs 0 and 1
        PCA1.setPWM_Duty_Cycle(PWM_VALUE_1, 0);
        PCA1.setPWM_Duty_Cycle(PWM_VALUE_2, 1);
        sleep_ms(20);
    }
}