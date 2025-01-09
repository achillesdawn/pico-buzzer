#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "shift_register/shift_register.h"

const uint8_t SER = 16;
const uint8_t SHCLK = 17;
const uint8_t RCLK = 18;

const uint8_t MOTOR = 19;


void init() {
    gpio_init(MOTOR);
    gpio_set_dir(MOTOR, GPIO_OUT);
    gpio_put(MOTOR, false);
}

int main() {

    stdio_init_all();
    init();

    sleep_ms(2000);

    printf("initializing\n");

    shift_register_t *sr = shift_register_init(SER, RCLK, SHCLK);

    gpio_put(MOTOR, true);

    while (true) {

        

        for (uint8_t i = 0; i < 9; i++) {
            printf("setting %d ", i);
            shift_register_set(sr, 1 << i);
            sleep_ms(100);
        }
    }
}
