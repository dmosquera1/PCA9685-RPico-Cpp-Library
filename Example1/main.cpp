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

int main() {

    stdio_init_all();

    i2c_init(i2c0, 100000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    PCA9685 PCA1(0x40, i2c0);

    uint8_t message[3][2] = {{0x00, 0x10}, {0x07, 0xFF}, {0x06, 0xFF}};
    uint8_t rawMessage[5][3] {{0x40, 0x00, 0x10}, {0x40, 0x07, 0xFF}, {0x40, 0x07, 0xFF}, {0x40, 0x08, 0xFF}, {0x40, 0x09, 0xFF}};
    int bytes = 0;


    while (1) {
            //PCA1.sendMessage(message);

            /*for (uint8_t i=0; i<0x7F; i++) {
                printf("%X:  %d\n", i, i2c_write_blocking(i2c0, i, message[1], 2, false));
            }
            */
            uint8_t message[2][2] = {{0x00, 0x10}, {0x07, 0xFF}};
            PCA1.setReg(message[0]);
            PCA1.setReg(message[1]);
            printf("%X\n", PCA1.readReg(0x00));
            /*i2c_write_raw_blocking(i2c0, rawMessage[1], 3);
            i2c_write_raw_blocking(i2c0, rawMessage[2], 3);
            i2c_write_raw_blocking(i2c0, rawMessage[3], 3);
            */
            /*printf("%d ", PCA1.sendMessage(message[1]));
            printf("%d ", PCA1.sendMessage(message[2]));
            printf("%d\n", PCA1.sendMessage(message[3]));
            */
            sleep_ms(1000);
    }
}