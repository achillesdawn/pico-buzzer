#include <inttypes.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buzzer.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

buzzer_t *buzzer_new(uint8_t pin) {

    buzzer_t *buzzer = calloc(1, sizeof(buzzer_t));
    if (buzzer == NULL) {
        return NULL;
    }
    buzzer->enabled = false;
    buzzer->pin = pin;
    buzzer->duty = 15;

    gpio_set_function(buzzer->pin, GPIO_FUNC_PWM);

    float notes[12] = {
        261, // 0 C
        277, // 1 C#
        293, // 2 D
        311, // 3 D#
        329, // 4 E
        349, // 5 F
        369, // 6 F#
        392, // 7 G
        415, // 8 G#
        440, // 9 A
        466, // 10 A#
        493, // 11 B
    };

    memcpy(buzzer->notes, notes, 12 * sizeof(float));

    buzzer->slice = pwm_gpio_to_slice_num(buzzer->pin);
    buzzer->channel = pwm_gpio_to_channel(buzzer->pin);
    return buzzer;
}

void buzzer_set_enabled(buzzer_t *buzzer, bool enabled) {
    pwm_set_enabled(buzzer->slice, enabled);
    buzzer->enabled = enabled;
}

void buzzer_toggle_enabled(buzzer_t *buzzer) {
    pwm_set_enabled(buzzer->slice, !buzzer->enabled);
    buzzer->enabled = !buzzer->enabled;
}

void buzzer_set_freq(buzzer_t *buzzer, uint32_t f) {
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);

    if (divider16 / 16 == 0) {
        divider16 = 16;
    }

    uint32_t wrap = clock * 16 / divider16 / f - 1;

    buzzer->wrap = wrap;

    pwm_set_clkdiv_int_frac(buzzer->slice, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(buzzer->slice, wrap);
    pwm_set_chan_level(
        buzzer->slice, buzzer->channel, wrap * buzzer->duty / 100
    );
}

void buzzer_set_volume(buzzer_t *buzzer, uint8_t volume) {
    pwm_set_chan_level(buzzer->slice, buzzer->channel, buzzer->wrap * volume);
}

void buzzer_play_scale(buzzer_t *buzzer) {
    for (uint8_t octave = 0; octave < 4; octave++) {
        for (uint32_t i = 0; i < 12; i++) {

            switch (i) {
            case 1:
            case 3:
            case 6:
            case 8:
            case 10:
                continue;
                break;
            default:
                break;
            }

            float freq = buzzer->notes[i] * pow(2, octave);

            printf("note %d: freq: %f hz\n", i, freq);

            buzzer_set_freq(buzzer, freq);

            if (!buzzer->enabled) {
                buzzer_set_enabled(buzzer, true);
            }
            sleep_ms(500);
        }
    }
}

// void buzzer_play_tone(buzzer_t *buzzer) {

//     buzzer_set_volume(buzzer, buzzer->duty);

//     float F = buzzer->notes[5] * 2 * 2 * 2;
//     float A = buzzer->notes[9] * 2 * 2 * 2;
//     float B = buzzer->notes[11] * 2 * 2 * 2;

//     buzzer_set_freq(buzzer, F);
//     sleep_ms(150);
//     buzzer_set_freq(buzzer, A);
//     sleep_ms(150);
//     buzzer_set_freq(buzzer, B);
//     sleep_ms(300);
//     buzzer_set_volume(buzzer, 0);
// }

void buzzer_play_tone(buzzer_t *buzzer) {

    buzzer_set_volume(buzzer, buzzer->duty);

    float F = buzzer->notes[5] * 2 * 2 * 2 *2 ;
    float A = buzzer->notes[9] * 2 * 2 * 2;
    float B = buzzer->notes[11] * 2 * 2 * 2;

    buzzer_set_freq(buzzer, B);
    sleep_ms(150);
    buzzer_set_volume(buzzer, 0);
    sleep_ms(150);
}