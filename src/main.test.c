#include <assert.h>
#include <stdio.h>

#include "blink.test.c"
#include "enable_pins.test.c"

int main() {
  assert(1 == 1);

  test_blink();
  test_enable_pins();

  printf("All Tests Passed!\n");
  return 0;
} 