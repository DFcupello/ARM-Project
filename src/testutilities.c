#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "utilities.h"
#include "testsuite.h"




void testGetNibble() {
  char name[] = "Get Nibble Test";
  int8_t bytes[] = {0, -78, 120};
  testInt8(getNibble(bytes[0], 0), 0, name);
  testInt8(getNibble(bytes[0], 1), 0, name);

  testInt8(getNibble(bytes[1], 0), 2, name);
  testInt8(getNibble(bytes[1], 1), 11, name);

  testInt8(getNibble(bytes[2], 0), 8, name);
  testInt8(getNibble(bytes[2], 1), 7, name);
}

void testGet2Bits() {
  char name[] = "Get 2 Bits Test";
  int8_t bytes[] = {-31, -8, 119};
  testInt8(get2Bits(bytes[0], 0), 1, name);
  testInt8(get2Bits(bytes[0], 1), 0, name);

  testInt8(get2Bits(bytes[1], 1), 2, name);
  testInt8(get2Bits(bytes[1], 2), 3, name);

  testInt8(get2Bits(bytes[2], 1), 1, name);
  testInt8(get2Bits(bytes[2], 2), 3, name);
}

void testAppendBytes() {
  char name[] = "Append Bytes Test";
  int8_t bytes1[][1] = {{6}, {-6}};
  int8_t bytes2[][2] = {{-4, 5}, {0, 8}};
  int8_t bytes3[][3] = {{7, 7, 7}, {-2, -5, -1}};
  int8_t bytes4[][4] = {{-8, -8, -8, -8}, {1, -5, 3, -8}};

  testInt32(appendBytes(bytes1[0], 1, 1), 6, name);
  testInt32(appendBytes(bytes1[1], 1, 1), 250, name);

  testInt32(appendBytes(bytes2[0], 2, 1), 1532, name);
  testInt32(appendBytes(bytes2[1], 2, 1), 2048, name);

  testInt32(appendBytes(bytes3[0], 3, 1), 460551, name);
  testInt32(appendBytes(bytes3[1], 3, 1), 16776190, name);

  testInt32(appendBytes(bytes4[0], 4, 1), -117901064, name);
  testInt32(appendBytes(bytes4[1], 4, 1), -133956863, name);

}

void testRotateRight() {
    char name[] = "Rotate Right Test";
    int32_t values[] = {3, 5, 20, 255, -320};
    testInt32(rotateRight(values[0], 0), 3, name);
    testInt32(rotateRight(values[1], 2), 1073741825, name);
    testInt32(rotateRight(values[2], 4), 1073741825, name);
    testInt32(rotateRight(values[3], 6), -67108861, name);
    testInt32(rotateRight(values[4], 8), -1056964610, name);

}

int main(void) {
    testGetNibble();
    testGet2Bits();
    testAppendBytes();
    testRotateRight();
    return 0;
}




