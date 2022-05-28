#include <stdio.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "enable_pins.c"
#include "fan_control.c"
#include "fan_drive.c"
#include "fan_monitor.c"

typedef struct BlinkValues {
  unsigned int on;
  unsigned int off;
} BlinkValues;

void set_fan_speed(uint fan_speed) {
    multicore_fifo_push_blocking(fan_speed);
    printf("Target fan percentage: %d\n", fan_speed);
}

const uint core1_TRANSISTOR_PIN = 22;
const int core1_number_of_gpio_pins = 1;
struct PinDefinition core1_pins [core1_number_of_gpio_pins];
core1_pins[0].pin_number = core1_TRANSISTOR_PIN;
core1_pins[0].in_or_out = GPIO_OUT;
int power_percentage = 80;

void core1_entry() {
    // Only for standard GPIO
    enable_pins(pins, number_of_gpio_pins, gpio_init, gpio_set_dir);
    while (true) {
        if (multicore_fifo_rvalid() == true) {
            power_percentage = multicore_fifo_pop_blocking();
        }
        pulse(TRANSISTOR_PIN, power_percentage, gpio_put, sleep_us);
    }
}


const uint RPM_PIN = 14;

bool get_fan_revolutions_second(struct repeating_timer *t){ 
    // This is actually bogus right now as we'll need to do pulse stretching to grabe the value
    // https://www.analog.com/en/analog-dialogue/articles/how-to-control-fan-speed.html
    printf("Tach Pin Reading: %d\n", get_fan_revolutions_since_reset(RPM_PIN));
    reset_revolutions(RPM_PIN);
}

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

struct repeating_timer timer;

const FAN_PERCENT_INCREMENT = 2;

int main() {
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    
    // Only for standard GPIO
    enable_pins(pins, number_of_gpio_pins, gpio_init, gpio_set_dir);

    setup_fan_monitor_pin(RPM_PIN, gpio_init, gpio_set_irq_enabled_with_callback, gpio_acknowledge_irq, GPIO_IRQ_LEVEL_HIGH);

    if (!add_repeating_timer_ms(1000, get_fan_revolutions_second, NULL, &timer)){
        gpio_put(ALARM_PIN, true);
    }

    while (true) {
        el_fan_control(FAN_PERCENT_INCREMENT, SPEED_UP_PIN, SPEED_DN_PIN, gpio_get, set_fan_speed);
        sleep_ms(100);
    }
}

