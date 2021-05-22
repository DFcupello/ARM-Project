#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "utilities.h"
#include "testsuite.h"

#define MEM_SIZE 16834

void testGetNBits() {
  char name[] = "Get N Bits Test";
  int numBits = 4;
  int32_t bytes4[] = {0, 178, 120};
  testInt32(getNBits(bytes4[0], numBits, 0), 0, name);
  testInt32(getNBits(bytes4[0], numBits, 4), 0, name);

  testInt32(getNBits(bytes4[1], numBits, 0), 2, name);
  testInt32(getNBits(bytes4[1], numBits, 4), 11, name);

  testInt32(getNBits(bytes4[2], numBits, 0), 8, name);
  testInt32(getNBits(bytes4[2], numBits, 4), 7, name);
  numBits = 2;
  int32_t bytes2[] = {225, 248, 119};
  testInt32(getNBits(bytes2[0], numBits, 0), 1, name);
  testInt32(getNBits(bytes2[0], numBits, 2), 0, name);

  testInt32(getNBits(bytes2[1], numBits, 2), 2, name);
  testInt32(getNBits(bytes2[1], numBits, 4), 3, name);

  testInt32(getNBits(bytes2[2], numBits, 2), 1, name);
  testInt32(getNBits(bytes2[2], numBits, 4), 3, name);
}

void testRotateRight() {
    char name[] = "Rotate Right Test";
    int32_t values[] = {3, 5, 20, 255, -320};
    bool dummy = false;
    testInt32(rotateRight(values[0], 0, &dummy), 3, name);
    testInt32(rotateRight(values[1], 2, &dummy), 1073741825, name);
    testInt32(rotateRight(values[2], 4, &dummy), 1073741825, name);
    testInt32(rotateRight(values[3], 6, &dummy), -67108861, name);
    testInt32(rotateRight(values[4], 8, &dummy), -1056964610, name);
}

void testBinaryLoader() {
  char name[] = "Binary Loader Test";
  char file[] = "/homes/dc1020/arm11_testsuite/test_cases/factorial";
  int32_t got[MEM_SIZE];
  binaryLoader(fopen(file, "rb"), file, false, got,  10);
  int32_t expected[] = {16818403, 84975843, -1862270240, 33595617, 17842658,20963, -83886310, 20750563, 2130917, 0};
  testManyInt32(got, expected, 10, name);
}

void testGetOffset() {
  char name[] = "Get Offset Test";
  int32_t input[] = {-476049407, -369098752};
  int32_t got[2];
  int size = sizeof(got) / sizeof(int);
  for (int i = 0; i < size; i++) {
    got[i] = getOffset(input[i]);
  }
  int32_t expected[] = {1, 0};
  testManyInt32(got, expected, size, name);
}


int main(void) {
    testGetNBits();
    testRotateRight();
    testBinaryLoader();
    testGetOffset();
    return 0;
}




