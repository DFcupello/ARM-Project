#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "testsuite.h"
#include <assert.h>
#include <string.h>

#define MAX_LINE_LENGTH 511

void testCond(bool ok, char *testname) {
    printf("T %s: %s\n", testname, ok ? "OK" : "FAIL");
}

void testInt32(uint32_t got, uint32_t expected, char *testname) {
    if (got == expected) {
        printf("T %s (got=%d): OK\n", testname, got);
    } else {
        printf("T %s (expected=%d, got=%d): FAIL\n", testname, expected, got);
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

void testTxtFile(FILE *gotFile, FILE *expectedFile, char *testname) {
    assert(gotFile && expectedFile);

    bool passed = true;
    char got[MAX_LINE_LENGTH];
    char expected[MAX_LINE_LENGTH];
    bool notDone = true;
    while (notDone) {
        bool gotOk = fgets(got, MAX_LINE_LENGTH, gotFile);
        bool expectedOk = fgets(expected, MAX_LINE_LENGTH, expectedFile);
        notDone = gotOk && expectedOk;
        if (gotOk ^ expectedOk) {
            passed = false;
        }
        if (notDone) {
            passed = passed && !strcmp(got, expected);
            if (strcmp(got, expected)) {
                printf("   %s (expected=%s, got=%s): FAIL\n", testname, expected, got);    
            }
        }
    }
    printf("T %s: %s\n", testname, passed ? "OK" : "FAIL" );
}

