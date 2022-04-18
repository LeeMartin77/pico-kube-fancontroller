#include <assert.h>
#include <stdio.h>

#include "blink.test.c"
#include "enable_pins.test.c"
#include "fan_monitor.test.c"

int main() {
  assert(1 == 1);

  test_blink();
  test_enable_pins();
  test_fan_monitor();

  printf("All Tests Passed!\n");
  return 0;
} 