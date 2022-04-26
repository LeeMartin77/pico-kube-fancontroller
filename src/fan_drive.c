#include <stdbool.h>

typedef void (*fn_gpio_put)(unsigned int,bool);

typedef void (*fn_sleep_ms)(unsigned int);

void pulse(unsigned int pin, unsigned int on_percentage, fn_gpio_put gpio_put, fn_sleep_ms sleep_ms) {
  assert(on_percentage <= 100);
  assert(on_percentage >= 0);
  int off_percentage = 100 - on_percentage;
  while (on_percentage + off_percentage > 0) {
    if(off_percentage) {
      gpio_put(pin, false);
      sleep_ms(1);
      off_percentage--;
    }
    if(on_percentage) {
      gpio_put(pin, true);
      sleep_ms(1);
      on_percentage--;
    }
  }
  gpio_put(pin, true);
}