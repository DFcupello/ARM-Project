#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "testsuite.h"

void testCond(bool ok, char *testname) {
    printf("T %s: %s\n", testname, ok ? "OK" : "FAIL");
}

void testInt32(uint32_t got, uint32_t expected, char *testname) {
    if (got == expected) {
        printf("T %s (got=%08x): OK\n", testname, got);
    } else {
        printf("T %s (expected=%08x, got=%08x): FAIL\n", testname, expected, got);
    }
}

void testInt8(uint8_t got, uint8_t expected, char *testname) {
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

void testManyInt32(uint32_t got[], uint32_t expected[], int size, char *testname) {
    bool passed = true;
    printf("T %s:\n", testname);
    for (int i = 1; i < size + 1; i++) {
        passed &= got[i - 1] == expected[i - 1];
        if (got[i - 1] == expected[i - 1]) {
        printf("  %2d. %s (got=%11d): OK\n", i, testname, got[i - 1]);
        } else {
        printf("   %d. %s (expected=%d, got=%d): FAIL\n", i, testname, expected[i - 1], got[i - 1]);
        }
    }
    printf("T %s: %s", testname, passed ? "OK\n" : "FAIL\n" );
}
