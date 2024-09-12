#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "7seg/7seg.h"
#include "pico/stdlib.h"

const uint8_t PIN_A = 1;
const uint8_t PIN_B = 2;
const uint8_t PIN_C = 3;
const uint8_t PIN_D = 4;

const uint32_t PIN_MASK =
    (1 << PIN_A) | (1 << PIN_B) | (1 << PIN_C) | (1 << PIN_D);

int main() {

    stdio_init_all();

    sleep_ms(2000);

    printf("initializing\n");

    gpio_init_mask(PIN_MASK);
    gpio_set_dir_out_masked(PIN_MASK);
    gpio_put_masked(PIN_MASK, PIN_MASK);

    seveng_seg_t *s = seven_seg_initialize(PIN_A, PIN_B, PIN_C, PIN_D);

    uint8_t value = 0;

    while (true) {

        printf("setting %d\n", value);

        seven_seg_set_value(s, value);
        value += 1;

        if (value > 9) {
            break;
        }

        sleep_ms(1000);
    }

    seven_seg_start_pio(s);
    
    while (true){
        tight_loop_contents();
    }
    
}
