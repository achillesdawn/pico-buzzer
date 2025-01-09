#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "shift_register/shift_register.h"

const uint8_t SER = 16;
const uint8_t RCLK = 17;
const uint8_t SRCLK = 18;

const uint8_t ADC_RESISTOR = 26;

int16_t map_range(
    int16_t from_min, int16_t from_max, int16_t to_min, int16_t to_max,
    int16_t value
) {
    int16_t range = from_max - from_min;

    int16_t a = value - from_min;
    float percent = (float)(a) / (float)range;

    return to_min + (int16_t)(percent * (float)to_max);
}

int main() {

    stdio_init_all();
    adc_init();

    adc_gpio_init(ADC_RESISTOR);
    adc_select_input(0);

    sleep_ms(1000);

    shift_register_t *sr = shift_register_init(SER, RCLK, SRCLK);

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t i = 0; i < 8; i++) {
            shift_register_set(sr, 1 << i);
            sleep_ms(30);
        }
    }
    shift_register_set(sr, 0);

    const float conversion_factor = 3.3f / (1 << 12);

    while (true) {

        uint16_t value = adc_read();
        int16_t level = map_range(2050, 4050, 0, 8, value);

        printf(
            "Raw value: %d, voltage: %f V level: %d\n",
            value,
            value * conversion_factor,
            level
        );

        uint8_t set_value = 0;
        for (uint8_t i = 0; i < level; i++) {
            set_value = set_value | (1 << i);
        }
        shift_register_set_inverted(sr, set_value);

        sleep_ms(30);
    }
}
