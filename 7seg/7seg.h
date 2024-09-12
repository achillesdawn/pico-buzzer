#include <inttypes.h>

typedef struct seven_seg {
    uint8_t pins[4];
    uint32_t pin_mask;

} seveng_seg_t;

seveng_seg_t *seven_seg_initialize(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

void seven_seg_set_value(seveng_seg_t *s, uint8_t value);

void seven_seg_start_pio(seveng_seg_t *s);

#pragma once

