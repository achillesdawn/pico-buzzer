#include <inttypes.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "shift_register.h"

shift_register_t *
shift_register_init(uint8_t SER, uint8_t RCLK, uint8_t SRCLK) {

    const uint32_t PIN_MASK = (1 << SER) | (1 << RCLK) | (1 << SRCLK);
    gpio_init_mask(PIN_MASK);
    gpio_set_dir_out_masked(PIN_MASK);
    gpio_put_masked(PIN_MASK, 0);

    shift_register_t *sr = malloc(sizeof(shift_register_t));
    sr->ser = SER;
    sr->rclk_storage_register_clock = RCLK;
    sr->srclk_shift_register_clock = SRCLK;
    sr->current_value = 0;
    return sr;
}

inline static void sleep_ns() {
    // at 125 MHZ each instruction takes 8ns
    // 4 instructions = 32ns
    // min switching time for shift register at 4.5v: 25ns
    for (uint8_t i = 0; i < 5; i++) {
        tight_loop_contents();
    }
}

inline static void shift_register_pulse(shift_register_t *sr) {
    gpio_put(sr->srclk_shift_register_clock, 1);
    sleep_ns();
    gpio_put(sr->srclk_shift_register_clock, 0);
    sleep_ns();
}

void shift_register_latch(shift_register_t *sr) {
    gpio_put(sr->rclk_storage_register_clock, 1);
    sleep_ns();
    gpio_put(sr->rclk_storage_register_clock, 0);
    sleep_ns();
}

void shift_register_push(shift_register_t *sr, bool value) {
    gpio_put(sr->ser, value);
    shift_register_pulse(sr);
}

void shift_register_set_masked(shift_register_t *sr,  uint8_t mask) {

    for (uint8_t i = 0; i < 8; i++) {
        bool bit = mask >> i;
        shift_register_push(sr, bit);
    }
    shift_register_latch(sr);
    return;
}

void shift_register_set(shift_register_t *sr,  bool buf[]) {

    for (uint8_t i = 0; i < 8; i++) {
        bool bit = buf[i];
        shift_register_push(sr, bit);
    }
    shift_register_latch(sr);
    return;
}

void shift_register_set_inverted(shift_register_t *sr,  bool buf[]) {

    for (int i = 7; i >= 0; i--) {
        bool bit = buf[i];
        shift_register_push(sr, bit);
    }
    shift_register_latch(sr);
    return;
}
