#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include "buzzer/buzzer.h"

const uint8_t LED_GREEN = 14;
const uint8_t BUZZER = 16;
const uint32_t PIN_MASK = (1u << LED_GREEN);

volatile bool led_state = false;

bool repeating_toogle_led() {
    led_state = !led_state;
    gpio_put(LED_GREEN, led_state);
    return true;
}

struct repeating_timer *setup_led() {
    printf("initializing timer\n");
    gpio_set_dir(LED_GREEN, true);
    gpio_put(LED_GREEN, true);

    struct repeating_timer *led_timer =
        calloc(1, sizeof(struct repeating_timer));

    add_repeating_timer_ms(500, repeating_toogle_led, NULL, led_timer);
    printf("timer initialized\n");
    return led_timer;
}

int main() {

    stdio_init_all();

    sleep_ms(2000);

    printf("initializing\n");

    gpio_init_mask(PIN_MASK);
    gpio_set_dir_out_masked(PIN_MASK);
    gpio_put_masked(PIN_MASK, PIN_MASK);

    struct repeating_timer *led_timer = setup_led();

    buzzer_t *buzzer = buzzer_new(BUZZER);
    buzzer_set_enabled(buzzer, true);

    while (true) {
        buzzer_play_tone(buzzer);
        buzzer_play_tone(buzzer);
        sleep_ms(10000);
    }
}
