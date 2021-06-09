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
    uint32_t bytes4[] = {0, 178, 120, 99};
    testInt32(getNBits(bytes4[0], numBits, 0), 0, name);
    testInt32(getNBits(bytes4[0], numBits, 4), 0, name);

    testInt32(getNBits(bytes4[1], numBits, 0), 2, name);
    testInt32(getNBits(bytes4[1], numBits, 4), 11, name);

    testInt32(getNBits(bytes4[2], numBits, 0), 8, name);
    testInt32(getNBits(bytes4[2], numBits, 4), 7, name);
    numBits = 2;
    uint32_t bytes2[] = {225, 248, 119};
    testInt32(getNBits(bytes2[0], numBits, 0), 1, name);
    testInt32(getNBits(bytes2[0], numBits, 2), 0, name);

    testInt32(getNBits(bytes2[1], numBits, 2), 2, name);
    testInt32(getNBits(bytes2[1], numBits, 4), 3, name);

    testInt32(getNBits(bytes2[2], numBits, 2), 1, name);
    testInt32(getNBits(bytes2[2], numBits, 4), 3, name);

    printf("%d\n", getNBits(bytes4[3], 8, 24));
}

void testRotateRight() {
    char name[] = "Rotate Right Test";
    uint32_t values[] = {3, 5, 20, 255, -320};
    bool dummy = false;
    testInt32(rotateRight(values[0], 0, &dummy), 3, name);
    testInt32(rotateRight(values[1], 2, &dummy), 1073741825, name);
    testInt32(rotateRight(values[2], 4, &dummy), 1073741825, name);
    testInt32(rotateRight(values[3], 6, &dummy), -67108861, name);
    testInt32(rotateRight(values[4], 8, &dummy), -1056964610, name);
}

void testArithmeticShiftRight() {
    char name[] = "Arithmetic Right Test";
    uint32_t values[] = {3, 5, 20, 255, 4294966976};
    bool dummy = false;
    testInt32(arithmeticShiftRight(values[0], 0, &dummy), 3, name);
    testInt32(arithmeticShiftRight(values[1], 2, &dummy), 1, name);
    testInt32(arithmeticShiftRight(values[2], 4, &dummy), 1, name);
    testInt32(arithmeticShiftRight(values[3], 6, &dummy), 3, name);
    testInt32(arithmeticShiftRight(values[4], 8, &dummy), -2, name);
}

void testLogicalShift() {
    char name[] = "Logical Shift Test";
    uint32_t values[] = {3, 5, 20, 255, 4294966976};
    bool dummy = false;
    // false means left, true means right
    testInt32(logicalShift(values[0], 0, true, &dummy), 3, name);
    testInt32(logicalShift(values[1], 2, false, &dummy), 20, name);
    testInt32(logicalShift(values[2], 4, true, &dummy), 1, name);
    testInt32(logicalShift(values[3], 6, false, &dummy), 16320, name);
    testInt32(logicalShift(values[4], 8, true, &dummy), 16777214, name);
}

void testPushPipeline() {
    char name[] = "Push Pipeline Test";
    uint32_t inputPipeline[] = {6, -12};
    uint32_t inputWord = 10;

    uint32_t expected[] = {10, 6};
    pushPipeline(inputPipeline, inputWord);
    testManyInt32(inputPipeline, expected, 2, name);
}

void testClearPipeline() {
    char name[] = "Clear Pipeline Test";
    uint32_t pipeline1[] = {88, -6};
    uint32_t pipeline2[] = {-1, -1};

    uint32_t expected[] = {-1, -1};
    clearPipeline(pipeline1);
    clearPipeline(pipeline2);
    testManyInt32(pipeline1, expected, 2, name);
    testManyInt32(pipeline2, expected, 2, name);
}

void testBinaryLoader() {
    char name[] = "Binary Loader Test";
    char file[] = "/homes/dc1020/arm11_testsuite/test_cases/factorial";
    uint32_t got[10];
    int dummy = 0;
    binaryLoader(fopen(file, "rb"), file, got, 10, &dummy);
    uint32_t expected[] = {16818403, 84975843, -1862270240, 33595617, 17842658, 20963, -83886310, 20750563, 2130917, 0};
    testManyInt32(got, expected, 10, name);
}

int main(void) {
    testGetNBits();
    testRotateRight();
    testLogicalShift();
    testBinaryLoader();
    testArithmeticShiftRight();
    testPushPipeline();
    testClearPipeline();
    return 0;
}
