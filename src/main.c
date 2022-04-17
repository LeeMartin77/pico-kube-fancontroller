/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "blink.c"
#include "enable_pins.c"

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    const uint TRANSCEIVER_PIN = 2;
    const int number_of_pins = 1;
    struct PinDefinition pins [number_of_pins];
    pins[0].pin_number = TRANSCEIVER_PIN;
    pins[0].in_or_out = 1;
    enable_pins(pins, number_of_pins, gpio_init, gpio_set_dir);
    while (true) {
        blink(TRANSCEIVER_PIN, 2000, 250, gpio_put, sleep_ms);
    }
#endif
}
