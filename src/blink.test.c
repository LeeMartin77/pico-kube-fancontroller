#include <assert.h>
#include <stdio.h>
#include "blink.c"

char call_sequence [4];

typedef struct {
  int pin_number;
  char value;
} gpioArgs;

int actual_gpio_callcount = 0;
gpioArgs actual_gpio_call_args [2];

int actual_sleep_callcount = 0;
int actual_sleep_calls [2];

void fake_gpio_put(unsigned int pin_number, char value) {
  actual_gpio_call_args[actual_gpio_callcount].pin_number = pin_number;
  actual_gpio_call_args[actual_gpio_callcount].value = value;
  call_sequence[actual_gpio_callcount + actual_sleep_callcount] = 'G';
  actual_gpio_callcount++;
}

void fake_sleep_ms(unsigned int duration) {
  actual_sleep_calls[actual_sleep_callcount] = duration;
  call_sequence[actual_gpio_callcount + actual_sleep_callcount] = 'S';
  actual_sleep_callcount++;
}

void test_blink() {
  assert(1 == 1);
  unsigned int expected_pin_number = 12;
  unsigned int expected_on_duration = 3849;
  unsigned int expected_off_duration = 7583;
  blink(expected_pin_number, expected_on_duration, expected_off_duration, fake_gpio_put, fake_sleep_ms);

  assert(actual_gpio_callcount == 2);
  assert(actual_sleep_callcount == 2);
  assert(actual_sleep_calls[0] == expected_on_duration);
  assert(actual_sleep_calls[1] == expected_off_duration);
  assert(actual_gpio_call_args[0].pin_number == expected_pin_number);
  assert(actual_gpio_call_args[0].value == 1);
  assert(actual_gpio_call_args[1].pin_number == expected_pin_number);
  assert(actual_gpio_call_args[1].value == 0);
  assert(call_sequence[0] == 'G');
  assert(call_sequence[1] == 'S');
  assert(call_sequence[2] == 'G');
  assert(call_sequence[3] == 'S');
}