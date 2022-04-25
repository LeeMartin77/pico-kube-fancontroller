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

void set_fan_speed(uint fan_speed) {
    uint off_value = 100 - fan_speed;
    multicore_fifo_push_blocking(fan_speed);
    multicore_fifo_push_blocking(off_value);
}

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
            blinkValues.on = multicore_fifo_pop_blocking();
            blinkValues.off = multicore_fifo_pop_blocking();
        }
        blink(TRANSISTOR_PIN, blinkValues.on, blinkValues.off, gpio_put, sleep_ms);
    }
}


const uint RPM_PIN = 14;

bool get_fan_revolutions_second(struct repeating_timer *t){ 
    printf("Revolutions Last Second: %d\n", get_fan_revolutions_since_reset(RPM_PIN));
    reset_revolutions(RPM_PIN);
}

int main() {
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    
    const uint SPEED_UP_PIN = 21;
    const uint SPEED_DN_PIN = 20;
    const uint ALARM_PIN = 10;
    const int number_of_gpio_pins = 2;
    struct PinDefinition pins [number_of_gpio_pins];
    pins[0].pin_number = SPEED_UP_PIN;
    pins[0].in_or_out = GPIO_IN;
    pins[1].pin_number = SPEED_DN_PIN;
    pins[1].in_or_out = GPIO_IN;
    pins[2].pin_number = ALARM_PIN;
    pins[2].in_or_out = GPIO_OUT;
    // Only for standard GPIO
    enable_pins(pins, number_of_gpio_pins, gpio_init, gpio_set_dir);


    setup_fan_monitor_pin(RPM_PIN, gpio_init, gpio_set_irq_enabled_with_callback, gpio_acknowledge_irq, GPIO_IRQ_LEVEL_HIGH);

    struct repeating_timer timer;
    if (!add_repeating_timer_ms(1000, get_fan_revolutions_second, NULL, &timer)){
        gpio_put(ALARM_PIN, true);
    }

    // Should be able to turn all of this into a big pile of testable code, not spaghetti

    bool last_speed_up = false;
    bool last_speed_dn = false;
    uint target_fan_speed = 50;
    const uint fan_speed_modifer = 5;
    while (true) {
        // TODO: Wrap this in tests maybe hmm?
        if (last_speed_up != gpio_get(SPEED_UP_PIN) > 0 && last_speed_up != true) {
            last_speed_up = true;
            if (target_fan_speed < 100) {
                target_fan_speed = target_fan_speed + fan_speed_modifer;
                set_fan_speed(target_fan_speed);
                printf("Target fan percentage: %d\n", target_fan_speed);
            }
        } else {
            last_speed_up = false;
        }

        if (last_speed_dn != gpio_get(SPEED_DN_PIN) > 0 && last_speed_dn != true) {
            last_speed_dn = true;
            if (target_fan_speed > 0) {
                target_fan_speed = target_fan_speed - fan_speed_modifer;
                set_fan_speed(target_fan_speed);
                printf("Target fan percentage: %d\n", target_fan_speed);
            }
        } else {
            last_speed_dn = false;
        }
    }
}

