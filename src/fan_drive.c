#include <stdbool.h>

typedef void (*fn_gpio_put)(unsigned int,bool);

typedef void (*fn_sleep_ms)(unsigned int);

void pulse(unsigned int pin, unsigned int on_duration, unsigned int off_duration, fn_gpio_put gpio_put, fn_sleep_ms sleep_ms) {
  gpio_put(pin, false);
  sleep_ms(off_duration);
  gpio_put(pin, true);
  sleep_ms(on_duration);
}