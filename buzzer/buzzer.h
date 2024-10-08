#include <inttypes.h>
#include <stdbool.h>

typedef struct buzzer {
    uint8_t pin;
    uint8_t slice;
    uint8_t channel;
    bool enabled;

    uint32_t wrap;
    uint8_t duty; // 0 - 100 as duty cycle

    float notes[12];
} buzzer_t;

buzzer_t *buzzer_new(uint8_t pin);
void buzzer_set_enabled(buzzer_t *buzzer, bool enabled);
void buzzer_toggle_enabled(buzzer_t *buzzer);
void buzzer_set_freq(buzzer_t *buzzer, uint32_t f);

void buzzer_play_tone(buzzer_t *buzzer);

#pragma once
