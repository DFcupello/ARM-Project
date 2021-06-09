// File by ap4220 Andrey Popov
// This is file has a testing purpose of the 
// instruction handler module

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inhandler.h"
#include "utilities.h"
#include "testsuite.h"
// #include "../test_framework/testsuite.h"

void testEndianConvertion(void) {
	int32_t expecteds[4] = {0xe3a00001, 0, 0x11002233, 0x12345678};
	int32_t inputs[4] = {0x0100a0e3, 0, 0x33220011, 0x78563412};

	for (int i = 0; i < 4; i++) {
		int32_t expected = expecteds[i];
		int32_t input = inputs[i];
		int32_t actual = littleEndToBigEnd(input);
		testInt32(actual, expected, "endian convertion");
	}
}

void testInstrSatisfyCond(void) {
	char name[] = "Instruction Cond Check Test";
	int8_t expecteds[8] = 
			{1, 0, 1, 0, 1, 0, 1, 1};
	int32_t cpsrs[8] = 
			{0x40000000, 0x40000000, 0x10000000, 0xc0000000,
			 0x10000000, 0xf0000000, 0x40000000, 0x00000000};
	int32_t instrs[8] = 
			{0x0100a0e3, 0x12345678, 0x11002233, 0x232123af,
			 0x332ee011, 0x9abcdef0, 0x9abcdef0, 0xe3a00001};

	int32_t instr, cpsr;
	bool expected, actual;
	for (int i = 0; i < 8; i++) {
		instr = instrs[i];
		cpsr = cpsrs[i];
		expected = (bool) expecteds[i];
		actual = instrSatisfyCond(instr, cpsr);
		testInt32(actual, expected, name);
	}
}



void testGetRegisters() {
    char destName[] = "Get Destination Register Test";
    char firstName[] = "Get First Operand Register Test";
    char secondName[] = "Get Second Operand Register Test";
    char sName[] = "Get Register S Test";
    char file[] = "../../arm11_testsuite/test_cases/factorial";
    uint32_t data[9];
    int dummy = 0;
    binaryLoader(fopen(file, "rb"), file, data, 9, &dummy);
    uint32_t gotDestRegister[9];
    uint32_t gotFirstOperand[9];
    uint32_t gotSecondOperand[9];
    uint32_t gotRegisterS[9];
    for (int i = 0; i < 9; i++) {
        int type = getInstType(littleEndToBigEnd(data[i]));
        gotDestRegister[i] = getDestinationRegister(littleEndToBigEnd(data[i]), type);
        gotFirstOperand[i] = getFirstOperandRegister(littleEndToBigEnd(data[i]), type);
        gotSecondOperand[i] = getSecondOperandRegister(littleEndToBigEnd(data[i]));
        gotRegisterS[i] = getRegisterS(littleEndToBigEnd(data[i]));
    }
    uint32_t expectedDestReg[] = {0, 1, 2, 0, 1, 0, 255, 3, 2};
    uint32_t expectedFirstReg[] = {0, 0, 0, 0, 1, 1, 255, 0, 3};
    uint32_t expectedSecondReg[] = {1, 5, 1, 2, 1, 0, 10, 1, 0};
    uint32_t expectedRegS[] = {0, 0, 0, 0, 0, 0, 15, 12, 0};
    testManyInt32(gotDestRegister, expectedDestReg, 9, destName);
    testManyInt32(gotFirstOperand, expectedFirstReg, 9, firstName);
    testManyInt32(gotSecondOperand, expectedSecondReg, 9, secondName);
    testManyInt32(gotRegisterS, expectedRegS, 9, sName);
}

void testGetOffset() {
    char name[] = "Get Offset Test";
    uint32_t input[] = {-476049407, -369098752};
    uint32_t got[2];
    int size = sizeof(got) / sizeof(int);
    for (int i = 0; i < size; i++)
    {
        got[i] = getOffset(input[i], getInstType(input[i]));
    }
    uint32_t expected[] = {1, 0};
    testManyInt32(got, expected, size, name);
}

int main(void) {
	testEndianConvertion();
	testInstrSatisfyCond();
	testGetRegisters();
	testGetOffset();
	return 0;
}