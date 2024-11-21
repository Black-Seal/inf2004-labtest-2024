#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

void init_gpio();
void init_pwm();
void main_fn();

// Interrupt callback for tracking number of taps
void button_callback(uint gpio, uint32_t events);

// Timer callback for 1-second updates
bool timer_callback(struct repeating_timer *rt);

// Setup PWM on a specific GPIO pin
void setup_pwm(uint gpio, uint dutycycle);

// Add a dot or dash
uint add_dotdash(uint dotdash);

// Playback update LED
void playback_update_led(uint led_id, uint led_state);

#endif
