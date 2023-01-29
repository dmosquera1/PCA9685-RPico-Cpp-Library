#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H
#include "pico/stdlib.h"

class RotaryEncoder {
    private:
        int8_t pinA, pinB, pinSW, state, counter = 2, update_log = 0, allow = 0;
        const int8_t quadrature_array[16] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};
    
    public:
        RotaryEncoder(int8_t A, int8_t B, int8_t SW) {
            pinA = A;
            pinB = B;
            pinSW = SW;
        }
        void encoder_update();
        int8_t encoder_direction();
        int8_t encoder_switch();
        int8_t encoder_quad();
};

#endif