#include <assert.h>
#include <stdio.h>

#include "blink.test.c"

int main() {
  assert(1 == 1);

  test_blink();

  printf("All Tests Passed!\n");
  return 0;
} 