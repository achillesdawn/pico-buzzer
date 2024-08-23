#include <inttypes.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "shift_register.h"

shift_register_t *
shift_register_init(uint8_t SER, uint8_t RCLK, uint8_t SHCLK) {

    const uint32_t PIN_MASK = (1 << SER) | (1 << RCLK) | (1 << SHCLK);
    gpio_init_mask(PIN_MASK);
    gpio_set_dir_out_masked(PIN_MASK);
    gpio_put_masked(PIN_MASK, 0);

    shift_register_t *sr = calloc(1, sizeof(shift_register_t));
    sr->ser = SER;
    sr->rclk_storage_register_clock = RCLK;
    sr->srclk_shift_register_clock = SHCLK;
    return sr;
}

void shift_register_pulse(shift_register_t *sr) {
    gpio_put(sr->srclk_shift_register_clock, 1);
    sleep_us(1);
    gpio_put(sr->srclk_shift_register_clock, 0);
    sleep_us(1);
}

void shift_register_latch(shift_register_t *sr) {
    gpio_put(sr->rclk_storage_register_clock, 1);
    sleep_us(1);
    gpio_put(sr->rclk_storage_register_clock, 0);
    sleep_us(1);
}

void shift_register_push(shift_register_t *sr, bool value) {
    gpio_put(sr->ser, value);
    shift_register_pulse(sr);
}

bool shift_register_set(shift_register_t *sr, uint8_t value) {
    for (uint i = 0; i < 8; i++) {
        bool bit = (value >> i) & 1;
        shift_register_push(sr, bit);
    }

    shift_register_latch(sr);
}