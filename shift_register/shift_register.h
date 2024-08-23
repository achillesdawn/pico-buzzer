#include <inttypes.h>
#include <stdbool.h>

typedef struct shift_register {
    uint8_t ser;
    uint8_t rclk_storage_register_clock;
    uint8_t srclk_shift_register_clock;
} shift_register_t;

shift_register_t *shift_register_init(uint8_t SER, uint8_t RCLK, uint8_t SHCLK);

bool shift_register_set(shift_register_t *sr, uint8_t value);

#pragma once