#include <assert.h>
#include <stdbool.h>
#include "fan_control.c"

void test_fan_control() {
  int fake_fc_gpio_get_call_input_int [100];
  int fake_fc_gpio_get_call_count = 0;

  int fake_pin_that_is_true = 0;
  int fake_fan_speed_modifier = 5;

  int fake_speed_up = 2;
  int fake_speed_dn = 4;

  int expected_fc_target_fan_speed = fc_target_fan_speed;

  unsigned int fake_fc_gpio_get(unsigned int input_int) {
    fake_fc_gpio_get_call_input_int[fake_fc_gpio_get_call_count] = input_int;
    fake_fc_gpio_get_call_count++;
    return fake_pin_that_is_true == input_int ? 1 : 0;
  }
  
  int fake_fc_set_fan_speed_input_speed [100];
  int fake_fc_set_fan_speed_count = 0;

  void fake_fc_set_fan_speed(unsigned int input_speed) {
    fake_fc_set_fan_speed_input_speed[fake_fc_set_fan_speed_count] = input_speed;
    fake_fc_set_fan_speed_count++;
  }

  // Nothing going on - shouldn't do anything, but will check pins
  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 2);
  assert(fake_fc_set_fan_speed_count == 0);

  // Pin Up is true, modifies
  fake_pin_that_is_true = fake_speed_up;
  expected_fc_target_fan_speed = expected_fc_target_fan_speed + fake_fan_speed_modifier;
  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 4);
  assert(fake_fc_set_fan_speed_count == 1);
  assert(fake_fc_set_fan_speed_input_speed[0] == expected_fc_target_fan_speed);

  // Pin is still true - doesn't refire
  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 6);
  assert(fake_fc_set_fan_speed_count == 1);

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 8);
  assert(fake_fc_set_fan_speed_count == 1);

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 10);
  assert(fake_fc_set_fan_speed_count == 1);

  // Pin released and repressed - refires

  fake_pin_that_is_true = 0;

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 12);
  assert(fake_fc_set_fan_speed_count == 1);

  fake_pin_that_is_true = fake_speed_up;
  expected_fc_target_fan_speed = expected_fc_target_fan_speed + fake_fan_speed_modifier;

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 14);
  assert(fake_fc_set_fan_speed_count == 2);
  assert(fake_fc_set_fan_speed_input_speed[1] == expected_fc_target_fan_speed);

  // Down Buttons
  // Nothing going on - shouldn't do anything, but will check pins

  fake_pin_that_is_true = 0;
  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 16);
  assert(fake_fc_set_fan_speed_count == 2);

  // Pin Up is true, modifies
  fake_pin_that_is_true = fake_speed_dn;
  expected_fc_target_fan_speed = expected_fc_target_fan_speed - fake_fan_speed_modifier;
  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 18);
  assert(fake_fc_set_fan_speed_count == 3);
  assert(fake_fc_set_fan_speed_input_speed[2] == expected_fc_target_fan_speed);

  // Pin is still true - doesn't refire
  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 20);
  assert(fake_fc_set_fan_speed_count == 3);

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 22);
  assert(fake_fc_set_fan_speed_count == 3);

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 24);
  assert(fake_fc_set_fan_speed_count == 3);

  // Pin released and repressed - refires

  fake_pin_that_is_true = 0;

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 26);
  assert(fake_fc_set_fan_speed_count == 3);

  fake_pin_that_is_true = fake_speed_dn;
  expected_fc_target_fan_speed = expected_fc_target_fan_speed - fake_fan_speed_modifier;

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  assert(fake_fc_gpio_get_call_count == 28);
  assert(fake_fc_set_fan_speed_count == 4);
  assert(fake_fc_set_fan_speed_input_speed[3] == expected_fc_target_fan_speed);

  // "Reset"

  fake_pin_that_is_true = 0;

  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get, fake_fc_set_fan_speed);

  // Both true at once - nothing changes

  unsigned int fake_fc_gpio_get_autotrue(unsigned int input_int) {
    return 1;
  }
  
  el_fan_control(fake_fan_speed_modifier, fake_speed_up, fake_speed_dn, fake_fc_gpio_get_autotrue, fake_fc_set_fan_speed);

  assert(fake_fc_set_fan_speed_count == 4);
}