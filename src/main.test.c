#include <assert.h>
#include <stdio.h>

#include "enable_pins.test.c"
#include "fan_control.test.c"
#include "fan_drive.test.c"
#include "fan_monitor.test.c"

int main() {
  assert(1 == 1);

  test_enable_pins();
  test_fan_control();
  test_fan_drive();
  test_fan_monitor();

  printf("All Tests Passed!\n");
  return 0;
} 