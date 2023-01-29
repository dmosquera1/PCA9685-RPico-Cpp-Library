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

    PCA1.setReg(0x00, 0xA0);
    PCA1.setReg(0x01, 0x04);
    PCA1.setReg(0x06, 0xFF);
    PCA1.setReg(0x07, 0x0F);
    PCA1.setReg(0x08, 0x0F);
    PCA1.setReg(0x09, 0x08);

    while (1) {
            printf("Register 0x06: %X\nRegister 0x07: %X\nRegister 0xFE: %X\n", PCA1.readReg(0x06), PCA1.readReg(0x07), PCA1.readReg(0xFE));
            sleep_ms(1000);
    }
}