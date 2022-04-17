#include <assert.h>
#include <stdio.h>
#include "enable_pins.c"

char ep_call_sequence [2];

typedef struct {
  int pin_number;
  char value;
} gpioArgsTwo;

int actual_gpio_set_dir_callcount = 0;
gpioArgsTwo actual_gpio_set_dir_call_args [2];

int actual_gpio_init_callcount = 0;
int actual_gpio_init_calls [2];

void fake_gpio_set_dir(unsigned int pin_number, char value) {
  actual_gpio_set_dir_call_args[actual_gpio_set_dir_callcount].pin_number = pin_number;
  actual_gpio_set_dir_call_args[actual_gpio_set_dir_callcount].value = value;
  ep_call_sequence[actual_gpio_set_dir_callcount + actual_gpio_init_callcount] = 'D';
  actual_gpio_set_dir_callcount++;
}

void fake_gpio_init(unsigned int pin) {
  actual_gpio_init_calls[actual_gpio_init_callcount] = pin;
  ep_call_sequence[actual_gpio_set_dir_callcount + actual_gpio_init_callcount] = 'I';
  actual_gpio_init_callcount++;
}

void test_enable_pins() {
  assert(1 == 1);
  unsigned int expected_pin_number = 12;
  struct PinDefinition pins [1];
  pins[0].pin_number = 12;
  pins[0].in_or_out = 1;
  enable_pins(pins, 1, fake_gpio_init, fake_gpio_set_dir);

  assert(actual_gpio_set_dir_callcount == 1);
  assert(actual_gpio_init_callcount == 1);
  assert(actual_gpio_init_calls[0] == expected_pin_number);
  assert(actual_gpio_set_dir_call_args[0].pin_number == expected_pin_number);
  assert(actual_gpio_set_dir_call_args[0].value == 1);
  assert(ep_call_sequence[0] == 'I');
  assert(ep_call_sequence[1] == 'D');
  
  actual_gpio_set_dir_callcount = 0;
  actual_gpio_init_callcount = 0;

  unsigned int expected_pin_one = 3;
  unsigned int expected_pin_two = 4;
  struct PinDefinition multi_pins [2];
  multi_pins[0].pin_number = expected_pin_one;
  multi_pins[0].in_or_out = 1;
  multi_pins[1].pin_number = expected_pin_two;
  multi_pins[1].in_or_out = 0;
  enable_pins(multi_pins, 2, fake_gpio_init, fake_gpio_set_dir);

  assert(actual_gpio_set_dir_callcount == 2);
  assert(actual_gpio_init_callcount == 2);
  assert(actual_gpio_init_calls[0] == expected_pin_one);
  assert(actual_gpio_set_dir_call_args[0].pin_number == expected_pin_one);
  assert(actual_gpio_set_dir_call_args[0].value == 1);
  assert(actual_gpio_init_calls[1] == expected_pin_two);
  assert(actual_gpio_set_dir_call_args[1].pin_number == expected_pin_two);
  assert(actual_gpio_set_dir_call_args[1].value == 0);
  assert(ep_call_sequence[0] == 'I');
  assert(ep_call_sequence[1] == 'D');
  assert(ep_call_sequence[2] == 'I');
  assert(ep_call_sequence[3] == 'D');
}