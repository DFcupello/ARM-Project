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
	       && ((instr & 0x0c000000) == 0);
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
	return (instr & 0x00100000) == 0x00100000;
}

/* Takes the 32-bit Big-endian instruction
	 Returns the opcode as 4 LSB of the 32 bit int
	 Applicable to Data Processing instructions only */
uint32_t opcode(uint32_t instr) {
	assert(instrIsDataProc(instr));
	return (instr & 0x01e00000) >> 21;
}

/* Takes 32-bit Big-endian instruction
   Returns the opcode as 4 LSB of the 32 bit int */
uint32_t condCode(uint32_t instr) {
	return (instr >> 28) & 0x0f;
}

/* Takes 32-bit value of CPSR register. 
   Returns true if 31st bit is 1 */
bool cpsr_N_flag(uint32_t cpsr) {
	return (cpsr & 0x80000000) == 0x80000000;
}
/* Takes 32-bit value of CPSR register. 
   Returns true if 30th bit is 1 */
bool cpsr_Z_flag(uint32_t cpsr) {
	return (cpsr & 0x40000000) == 0x40000000;
}

/* Takes 32-bit value of CPSR register. 
   Returns true if 29th bit is 1 */
bool cpsr_C_flag(uint32_t cpsr) {
	return (cpsr & 0x20000000) == 0x20000000;
}

/* Takes 32-bit value of CPSR register. 
   Returns true if 28th bit is 1 */
bool cpsr_V_flag(uint32_t cpsr) {
	return (cpsr & 0x10000000) == 0x10000000;
}

/* Takes Big-endian instruction and CPSR value.
   Returns the result of flag boolean expression 
   which depends on the condcode 
   PRE: condcode != 1111 */
bool instrSatisfyCond(uint32_t instr, uint32_t cpsr) {
	uint32_t cond = condCode(instr);
	bool n = cpsr_N_flag(cpsr);
	bool z = cpsr_Z_flag(cpsr);
	bool c = cpsr_C_flag(cpsr);
	bool v = cpsr_V_flag(cpsr);

	bool res;
	switch (cond) {   // code suffix
		case 0:         // 0000   EQ     
			res = z;
			break;
		case 1:         // 0001   NE
			res = !z;
			break;
		case 2:         // 0010   CS
			res = c;
			break;
		case 3:         // 0011   CC     
			res = !c;
			break;
		case 4:         // 0100   MI
			res = n;
			break;
		case 5:         // 0101   PL
			res = !n;
			break;
		case 6:         // 0110   VS     
			res = v;
			break;
		case 7:         // 0111   VC
			res = !v;
			break;
		case 8:         // 1000   HI
			res = c && !z;
			break;
		case 9:         // 1001   LS     
			res = !c && z;
			break;
		case 10:        // 1010   GE
			res = n == v;
			break;
		case 11:        // 1011   LT
			res = n != v;
			break;
		case 12:        // 1100   GT     
			res = (n == v) && !z;
			break;
		case 13:        // 1101   LE
			res = (n != v) || z;
			break;
		case 14:        // 1110   AL or no suffix
			res = true;
			break;
	}
	// code 1111 is reserved and must not be used
	return res;
}


