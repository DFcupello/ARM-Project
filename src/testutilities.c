#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "utilities.h"
#include "testsuite.h"
#include "inhandler.h"
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
    testInt32(logicalShift(values[2], 4, true, &dummy), 1 , name);
    testInt32(logicalShift(values[3], 6, false, &dummy), 16320, name);
    testInt32(logicalShift(values[4], 8, true, &dummy), 16777214, name);
}

void testBinaryLoader() {
  char name[] = "Binary Loader Test";
  char file[] = "/homes/dc1020/arm11_testsuite/test_cases/factorial";
  uint32_t got[10];
  binaryLoader(fopen(file, "rb"), file, got,  10);
  uint32_t expected[] = {16818403, 84975843, -1862270240, 33595617, 17842658, 20963, -83886310, 20750563, 2130917, 0};
  testManyInt32(got, expected, 10, name);
}

void testGetRegisters() {
  char destName[] = "Get Destination Register Test";
  char firstName[] = "Get First Operand Register Test";
  char secondName[] = "Get Second Operand Register Test";
  char sName[] = "Get Register S  Test";
  char file[] = "/homes/dc1020/arm11_testsuite/test_cases/factorial";
  uint32_t data[10];
  binaryLoader(fopen(file, "rb"), file, data, 10);
  uint32_t gotDestRegister[10];
  uint32_t gotFirstOperand[10];
  uint32_t gotSecondOperand[10];
  uint32_t gotRegisterS[10];
  for (int i = 0; i < 10; i++) {
    gotDestRegister[i] = getDestinationRegister(littleEndToBigEnd(data[i]));
    gotFirstOperand[i] = getFirstOperandRegister(littleEndToBigEnd(data[i]));
    gotSecondOperand[i] = getSecondOperandRegister(littleEndToBigEnd(data[i]));
    gotRegisterS[i] = getRegisterS(littleEndToBigEnd(data[i]));
  }
  uint32_t expectedDestReg[] = {0, 1, 2, 0, 1, 0, 255, 3, 2, 255};
  uint32_t expectedFirstReg[] = {0, 0, 0, 0, 1, 1, 255, 0, 3, 255};
  uint32_t expectedSecondReg[] = {255, 255, 1, 2, 255, 255, 255, 255, 255, 255};
  uint32_t expectedRegS[] = {255, 255, 1, 255, 255, 255, 255, 255, 255, 255};
  testManyInt32(gotDestRegister, expectedDestReg, 10, destName);
  testManyInt32(gotFirstOperand, expectedFirstReg, 10, firstName);
  testManyInt32(gotSecondOperand, expectedSecondReg, 10, secondName);
  testManyInt32(gotRegisterS, expectedRegS, 10, sName);
}

void testGetOffset() {
  char name[] = "Get Offset Test";
  uint32_t input[] = {-476049407, -369098752};
  uint32_t got[2];
  int size = sizeof(got) / sizeof(int);
  for (int i = 0; i < size; i++) {
    got[i] = getOffset(input[i]);
  }
  uint32_t expected[] = {1, 0};
  testManyInt32(got, expected, size, name);
}

void testPushPipeline() {
  char name[] = "Push Pipeline Test";
  uint32_t inputPipeline[] = {6, -12, 57};
  uint32_t inputWord = 10;
  
  uint32_t expected[] = {10, 6, -12};
  pushPipeline(inputPipeline, inputWord);
  testManyInt32(inputPipeline, expected, 3, name);

}

void testIsPipelineFull() {
  char name[] = "Is Pipeline Full Test";
  uint32_t pipeline1[] = {-1, 3, -1};
  uint32_t pipeline2[] = {-1, -1, 0};
  uint32_t pipeline3[] = {-1, -1, -1};

  testCond(!isPipelineFull(pipeline1), name);
  testCond(!isPipelineFull(pipeline2), name);
  testCond(isPipelineFull(pipeline3), name);
}

void testClearPipeline() {
  char name[] = "Clear Pipeline Test";
  uint32_t pipeline1[] = {88, -6, 0};
  uint32_t pipeline2[] = {-1, -1, -1};

  uint32_t expected[] = {-1, -1, -1};
  clearPipeline(pipeline1);
  clearPipeline(pipeline2);
  testManyInt32(pipeline1, expected, 3, name);
  testManyInt32(pipeline2, expected, 3, name);
}



int main(void) {
    testGetNBits();
    // testRotateRight();
    // testBinaryLoader();
    // testGetOffset();
    // testLogicalShift();
    // testArithmeticShiftRight();
    // testGetRegisters();
    // testPushPipeline();
    // testIsPipelineFull();
    // testClearPipeline();
    return 0;
}




