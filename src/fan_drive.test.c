#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "fan_drive.c"

void test_fan_drive() {
  unsigned int expected_pin_number = 12;

  unsigned int total_sleep_duration = 0;
  unsigned int sleep_durations [2];
  sleep_durations[0] = 0;
  sleep_durations[1] = 0;
  bool pin_on = 0;

  unsigned int expected_on_duration = 50;
  unsigned int expected_off_duration = 50;

  void fake_gpio_put(unsigned int pin_number, bool value) {
    assert(expected_pin_number == pin_number);
    pin_on = value;
  }

  void fake_sleep_ms(unsigned int duration) {
    total_sleep_duration = total_sleep_duration + duration;
    const int pin_index = pin_on ? 1 : 0;
    sleep_durations[pin_index] = sleep_durations[pin_index] + duration;
    // Ensure that we only have more than ms off if we have to
    if(!pin_on && expected_off_duration <= ((expected_off_duration + expected_on_duration)/2)) {
      assert(duration < 2);
    }
  }
  for (int i = 0; i <= 100; i++) {
    total_sleep_duration = 0;
    sleep_durations[0] = 0;
    sleep_durations[1] = 0;

    expected_on_duration = i;
    expected_off_duration = 100 - i;
    pin_on = 0;

    pulse(expected_pin_number, expected_on_duration, fake_gpio_put, fake_sleep_ms);

    assert(total_sleep_duration == (expected_on_duration + expected_off_duration));
    assert(sleep_durations[1] == expected_on_duration);
    assert(sleep_durations[0] == expected_off_duration);
    // MAke sure pin is pulled high at end unles spower is 0
    assert(pin_on);
  }
}