#include "RotaryEncoder.hpp"
#include "pico/stdlib.h"

void RotaryEncoder::encoder_update() {
    state <<= 2;
    state |= gpio_get(pinA)*2 + gpio_get(pinB);
    state &= 0x0f;
    counter += quadrature_array[state];
}

int8_t RotaryEncoder::encoder_direction() {
    if (counter==2) {
        allow = !allow;
        counter = 0;
        return 1*allow;
    } else if (counter==-2) {
        allow = !allow;
        counter = 0;
        return -1*allow;
    }
    return 0;
}

int8_t RotaryEncoder::encoder_switch() {
    return (!gpio_get(pinSW));
}

int8_t RotaryEncoder::encoder_quad() {
    return state;
}