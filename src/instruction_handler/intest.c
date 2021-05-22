// File by ap4220 Andrey Popov
// This is file has a testing purpose of the 
// instruction handler module

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inhandler.h"

void testEndianConvertion() {
	int TEST_CASES_NUM = 4;

	int32_t expecteds[] = {0xe3a00001, 0, 0x11002233, 0x12345678};
	int32_t inputs[] = {0x0100a0e3, 0, 0x33220011, 0x78563412};

	for (int i = 0; i < TEST_CASES_NUM; i++) {
		int32_t expected = expecteds[i];
		int32_t input = inputs[i];
		int32_t actual = littleEndToBigEnd(input);
		printf("T endian convertion %x --> %x", expected, actual);
		if (actual == expected) {
			printf(" OK!");
		} else {
			printf(" BAD!");
		}
		printf("\n");
	}
}

void testInstructionTypeDetermination() {
	// TODO
}

int main(void) {
	testEndianConvertion();
	return 0;
}