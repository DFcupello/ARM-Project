// File by ap4220 Andrey Popov
// This module supposed to provide functions

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "inhandler.h"

#define RIGHT_MOST_BYTE_SELECTOR 0x000000ff
#define MID_RIGHT_BYTE_SELECTOR 0x0000ff00
#define LEFT_MOST_BYTE_SELECTOR 0xff000000
#define MID_LEFT_BYTE_SELECTOR 0x00ff0000
#define INSTR_SIZE 4 // bytes

/* Converts 32bit instruction from little-endian byte order
   into the Big-endian order using shifting and bitwise operators
   For example, the first line of factorial program page 17 of spec
   inputs  - 00000001 00000000 10100000 11100011
   returns - 11100011 10100000 00000000 00000001 */
uint32_t littleEndToBigEnd(uint32_t origInstr) {

	uint32_t selectors[INSTR_SIZE] = {
		RIGHT_MOST_BYTE_SELECTOR,
		MID_RIGHT_BYTE_SELECTOR,
		MID_LEFT_BYTE_SELECTOR,
		LEFT_MOST_BYTE_SELECTOR
	};
	uint32_t result = 0;
	uint32_t currentByte;
	for (int i = 0, shifter = 24; i < 4; shifter -= 16, i++) {
		currentByte = origInstr & selectors[i];
		if (shifter > 0) {
			currentByte = currentByte << shifter;
		} else {
			currentByte = currentByte >> (-1 * shifter);
		}
		result = result | currentByte;
	}
	return result;
}

uint32_t bigEndToLittleEnd(uint32_t origInstr) {
	return littleEndToBigEnd(origInstr);
}