// File by ap4220 Andrey Popov
// This is file has a testing purpose of the 
// instruction handler module

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inhandler.h"
// #include "../test_framework/testsuite.h"

void testEndianConvertion(void) {
	int32_t expecteds[4] = {0xe3a00001, 0, 0x11002233, 0x12345678};
	int32_t inputs[4] = {0x0100a0e3, 0, 0x33220011, 0x78563412};

	for (int i = 0; i < 4; i++) {
		int32_t expected = expecteds[i];
		int32_t input = inputs[i];
		int32_t actual = littleEndToBigEnd(input);
		// testInt32(actual, expected, "endian convertion");
		printf("T endian convertion %x <--> %x", expected, actual);
		if (actual == expected) {
			printf(" OK!");
		} else {
			printf(" BAD!");
		}
		printf("\n");
	}
}

void testInstructionTypeDetermination(void) {
	// TODO
}

void testInstrSatisfyCond(void) {
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
		printf("T instr satisfy cond %d <--> %d", expected, actual);
		if (actual == expected) {
			printf(" OK!");
		} else {
			printf(" BAD!");
		}
		printf("\n");
	}
}

int main(void) {
	// testEndianConvertion();
	testInstrSatisfyCond();
	return 0;
}