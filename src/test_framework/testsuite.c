#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "testsuite.h"

void testCond(bool ok, char *testname) {
  printf("T %s: %s\n", testname, ok ? "OK" : "FAIL");
}

void testInt32(int32_t got, int32_t expected, char *testname) {
  if (got == expected) {
    printf("T %s (got=%d): OK\n", testname, got);
  } else {
    printf("T %s (expected=%d, got=%d): FAIL\n", testname, expected, got);
  }
}

void testInt8(int8_t got, int8_t expected, char *testname) {
  if (got == expected) {
    printf("T %s (got=%d): OK\n", testname, got);
  } else {
    printf("T %s (expected=%d, got=%d): FAIL\n", testname, expected, got);
  }
}

void testInt(int got, int expected, char *testname) {
  if (got == expected) {
    printf("T %s (got=%d): OK\n", testname, got);
  } else {
    printf("T %s (expected=%d, got=%d): FAIL\n", testname, expected, got);
  }
}




