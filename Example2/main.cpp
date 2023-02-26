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
double degree_value;

//Diagnostic Timer Callback
volatile bool timer_fired = false;
bool alarm_callback(struct repeating_timer *t) {
    printf("Servo2_Off:  %d    Servo3_Off: %d\n", PCA1.getPWM_OFF(2), PCA1.getPWM_OFF(3));
    printf("Degrees: %lf", degree_value);
    if (gpio_get(Onboard_LED)) {
        gpio_put(Onboard_LED, false);
    } else {
        gpio_put(Onboard_LED,true);
    }
    return true;
}

//Sets the angle of a servo connected to PWM "PWM_INDEX" of PCA9685 "PCA" to the angle of "degrees" given an array of 4 double values that carry the following data:{minDutyCycle, maxDutyCycle, maxAngle, DutyCyclePerDegree}
void Set_Servo_Angle(PCA9685 PCA, uint8_t PWM_INDEX, double degrees, double* servoData) {
    double PWM_Value;
    if (degrees <= servoData[2]) {
        PWM_Value = degrees*servoData[3];
        PWM_Value += servoData[0];
        PCA.setPWM_Duty_Cycle(PWM_Value, PWM_INDEX);
    }
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

    /*
    ***USING THE ES08MA II ANALOG SERVO***

    0.0275 and 0.1225 duty cycles seem to be the limits that the servo will hold an angle and the difference is 180 degrees

    Angle seems to change linearly with changes in duty cycle between these two points
    therefore duty cycle % per degree seems to be 0.053% (0.00053) per degree

    A sample function that maps degrees to duty cycle is defined above and is one example of how to control a servo in terms of degrees
    */

    //Creating an array of values that hold the servo's duty cycle bounds and degree range
    double ES08MA_II[4] = {0.0275, 0.1225, 180.0, 0.00053};

    degree_value = 0;
    int8_t sweepUp = 1;

    while (1) {
        if (degree_value >= 180) sweepUp = -1;
        if (degree_value <= 0) sweepUp = 1;
        
        degree_value += 45*sweepUp;

        Set_Servo_Angle(PCA1, 2, degree_value, ES08MA_II);
        Set_Servo_Angle(PCA1, 3, 180.0-degree_value, ES08MA_II);
        sleep_ms(1000);
    }
}