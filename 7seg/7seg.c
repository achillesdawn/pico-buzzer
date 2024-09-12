#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "7seg.h"
#include "7seg.pio.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"

seveng_seg_t *seven_seg_initialize(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    seveng_seg_t *s = malloc(sizeof(seveng_seg_t));

    s->pins[0] = a;
    s->pins[1] = b;
    s->pins[2] = c;
    s->pins[3] = d;
    s->pin_mask = (1 << a) | (1 << b) | (1 << c) | (1 << d);
    return s;
}

void seven_seg_set_value(seveng_seg_t *s, uint8_t value) {
    if (value > 9) {
        return;
    }

    uint32_t value_mask = 0;

    for (uint8_t i = 0; i < 4; i++) {

        bool set_value = (value >> i) & 1;

        printf("%d", set_value);

        value_mask |= (set_value << s->pins[i]);
    }

    printf(
        "from set value: pin_mask: %d, value mask %d\n", s->pin_mask, value_mask
    );

    gpio_put_masked(s->pin_mask, value_mask);
}

void seven_seg_start_pio(seveng_seg_t *s) {

    // using either pio0 or pio1
    PIO pio = pio0;

    // offset is the memory location of the program in the pio space
    uint offset = pio_add_program(pio, &sevenseg_program);
    // get one of the 4 state machines of the pio
    uint state_machine = pio_claim_unused_sm(pio, true);

    sevenseg_program_init(pio, state_machine, offset, s->pins[0]);
    pio_sm_put_blocking(pio, state_machine, (1<<8) | 5u);

}

void seven_seg_pio_set_value() {}
