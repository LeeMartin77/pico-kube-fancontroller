#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "blink.c"
#include "enable_pins.c"
#include "fan_monitor.c"
#include "pico/multicore.h"


typedef struct BlinkValues {
  unsigned int on;
  unsigned int off;
} BlinkValues;


void core1_entry() {
    const uint TRANSISTOR_PIN = 22;
    const int number_of_gpio_pins = 1;
    struct PinDefinition pins [number_of_gpio_pins];
    pins[0].pin_number = TRANSISTOR_PIN;
    pins[0].in_or_out = GPIO_OUT;
    // Only for standard GPIO
    enable_pins(pins, number_of_gpio_pins, gpio_init, gpio_set_dir);
    struct BlinkValues blinkValues;
    blinkValues.on = 75;
    blinkValues.off = 25;
    while (true) {
        if (multicore_fifo_rvalid() == true) {

        }
        blink(TRANSISTOR_PIN, blinkValues.on, blinkValues.off, gpio_put, sleep_ms);
    }
}

int main() {
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    const uint RPM_PIN = 14;
    setup_fan_monitor_pin(RPM_PIN, gpio_init, gpio_set_irq_enabled_with_callback, gpio_acknowledge_irq);
    while (true) {
        printf("Pulses since last message: %d\n", get_fan_revolutions_since_reset(RPM_PIN));
        reset_revolutions(RPM_PIN);
        sleep_ms(1000);
    }
}

