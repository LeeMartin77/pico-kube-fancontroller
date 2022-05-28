#include <stdbool.h>

typedef void (*fn_gpio_put)(unsigned int,bool);

typedef void (*fn_sleep_ms)(unsigned int);

int off_percentage = 100;
int _pulse_i = 0;

void pulse(unsigned int pin, unsigned int on_percentage, fn_gpio_put gpio_put, fn_sleep_ms sleep_ms) {
  assert(on_percentage <= 100);
  assert(on_percentage >= 0);
  // Carve out the edge cases
  if (on_percentage == 0) {
    gpio_put(pin, false);
    sleep_ms(100);
    return;
  }
  if (on_percentage == 100) {
    gpio_put(pin, true);
    sleep_ms(100);
    return;
  }

  off_percentage = 100 - on_percentage;
  for (_pulse_i = 0; _pulse_i <= 100; _pulse_i++) {
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