#include <assert.h>
#include "blink.c"

void fake_gpio_put(unsigned int pin_number, char value) {

}

void fake_sleep_ms(unsigned int duration) {
  
}

void test_blink() {
  assert(1 == 1);

  blink(1, 100, 300, fake_gpio_put, fake_sleep_ms);
}