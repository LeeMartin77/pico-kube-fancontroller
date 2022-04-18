/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "blink.c"
#include "enable_pins.c"
#include "fan_monitor.c"

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    stdio_init_all();
    const uint TRANSCEIVER_PIN = 2;
    const uint RPM_PIN = 3;
    const int number_of_pins = 2;
    struct PinDefinition pins [number_of_pins];
    pins[0].pin_number = TRANSCEIVER_PIN;
    pins[0].in_or_out = GPIO_OUT;
    // Only for standard GPIO
    enable_pins(pins, number_of_pins, gpio_init, gpio_set_dir);
    setup_fan_monitor_pin(RPM_PIN, gpio_init, gpio_set_irq_enabled_with_callback, gpio_acknowledge_irq);
    while (true) {
        printf("Pulses since last message: %d\n", get_fan_revolutions_since_reset(RPM_PIN));
        reset_revolutions(RPM_PIN);
        blink(TRANSCEIVER_PIN, 2000, 250, gpio_put, sleep_ms);
    }
#endif
}

