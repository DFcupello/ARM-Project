// File by ap4220 Andrey Popov
// This module includes utility functions for
// working with the instructions.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
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
	for (int i = 0, shifter = 24; i < INSTR_SIZE; shifter -= 16, i++) {
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

/* Takes the 32-bit Big-endian instruction. 
   Returns true, if the instruction bits 27-26 are 00
   and instruction is not a Multiply type */
bool instrIsDataProc(uint32_t instr) {
	return !instrIsMultiply(instr)
	       && ((instr & 0x0c00000) == 0);
}

/* Takes the 32-bit Big-endian instruction. 
   Returns true, if the instruction 
   all bits 27-22 are 0 and bits 7-4 are 1001 */
bool instrIsMultiply(uint32_t instr) {
	return (instr & 0x0fc000f0) == 0x00000090;
}

/* Takes the 32-bit Big-endian instruction. 
   Returns true, if the instruction 
   bits 27-26 are 01 and bits 22-21 are 00 */
bool instrIsSingleDataTrans(uint32_t instr) {
	return (instr & 0x0c600000) == 0x04000000;
}

/* Takes the 32-bit Big-endian instruction.
   Returns true, if bits 27-24 are 1010 respectively,
   i.e. hex representation is 0x.a...... */
bool instrIsBranch(uint32_t instr) {
	return (instr & 0x0f000000) == 0x0a000000;
}

/* Takes the 32-bit Big-endian instruction.
   Returns true if the 25th bit is 1.
   Applicable to Data Processing or
   Single Data Transfer type only */
bool isIFlagSet(uint32_t instr) {
	assert(instrIsDataProc(instr) || instrIsSingleDataTrans(instr));
	return (instr & 0x02000000) == 0x02000000;
}

/* Takes the 32-bit Big-endian instruction.
   Returns true if the 20th bit is 1.
   Applicable to Data Processing or Multiply only */
bool isSFlagSet(uint32_t instr) {
	assert(instrIsDataProc(instr) || instrIsMultiply(instr));
	return (instr & 0x00100000) == 0x00100000;
}

/* Takes the 32-bit Big-endian instruction.
   Returns true if the 21st bit is 1. 
   Applicable to Multiply type only*/
bool isAFlagSet(uint32_t instr) {
	assert(instrIsMultiply(instr));
	return (instr & 0x00200000) == 0x00200000;
}

/* Takes the 32-bit Big-endian instruction.
   Returns true if the 24th bit is 1.
   Applicable to Multiply type only */
bool isPFlagSet(uint32_t instr) {
	assert(instrIsSingleDataTrans(instr));
	return (instr & 0x01000000) == 0x01000000;
}

/* Takes the 32-bit Big-endian instruction.
   Returns true if the 23rd bit is 1.
   Applicable to Multiply type only */
bool isUFlagSet(uint32_t instr) {
	assert(instrIsSingleDataTrans(instr));
	return (instr & 0x00800000) == 0x00800000;
}

/* Takes the 32-bit Big-endian instruction.
   Returns true if the 20th bit is 1.
   Applicable to Multiply type only */
bool isLFlagSet(uint32_t instr) {
	assert(instrIsSingleDataTrans(instr));
	return (instr & 0x00100000) == 0x01000000;
}
