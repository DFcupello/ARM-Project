
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include "utilities.h"
#include "inhandler.h"

#define REG_SIZE 17

/* 
  Gets nth nibble (0 is LSN, 1 is MSN)
  precondition: n is of range 0-1
  postcondition: r = nth nibble
*/

int8_t getNibble(int8_t byte, int n) {
  assert (n >= 0 && n <= 1);  
  int32_t mask = 15;
  int32_t answer = 0;
  mask = mask << 4 * n;
  answer = mask & byte;
  answer = answer >> 4 * n;
  assert (answer >= 0 && answer <= 15);
  return (int8_t) answer;
}

/* 
  Gets nth 2-bits (0 is LSb, 3 is MSb)
  precondition: n is of range 0-3
  postcondition: r = nth 2-bit
*/

int8_t get2Bits(int8_t byte, int n) {
  assert (n >= 0 && n <= 3);  
  int32_t mask = 3;
  int32_t answer = 0;
  mask = mask << 2 * n;
  answer = mask & byte;
  answer = answer >> 2 * n;
  return (int8_t) answer;
}

/*
  Appends n bytes together (0 is LSb)
  Example in big Endian: Input: {0101, 1111, 1001} Output: 0000100111110101
*/
int32_t appendBytes(int8_t bytes[], int size, bool bigEndian) {
  assert (size >= 0 && size <= 4);
  uint32_t word = 0;
  for (int i = 0; i < size; i++) {
    word |= ((uint8_t) bytes[bigEndian ? i : ((size - 1) - i)]) << (8 * i);
  }
  return (int32_t) word;
}

/*
Gets the type of instruction from a given word.
Must be big endian

*/
enum InstType getInstType(int8_t bytes[]) {
  int8_t bits = get2Bits(bytes[3], 1);
  assert (bits >= 0 && bits <= 2);
  if (bits == 2) {
    return Branch;
  }
  else if (bits == 1) {
    return Transfer;
  }
  else if (bits == 0) {
    if (getNibble(bytes[0], 1) == 9 && getNibble(bytes[3], 0) == 0 && get2Bits(bytes[2], 3) == 0) {
      return Mul;
    }
    if (appendBytes(bytes, 4, true) == 0) {
      return HALT;
    }
    return Data;
  }
  return -1;
}


/*
Gets the destination register from the instruction, assuming it is not branching.
precondition: big-endian word
postcondition: r = Rdest or r = 255 (if branching or halting)
*/
int8_t getDestinationRegister(int8_t bytes[]) {
  enum InstType type = getInstType(bytes);
  int8_t reg;
  switch (type) {
    case Data: reg = getNibble(bytes[1], 1);
    break;
    case Mul: reg = getNibble(bytes[2], 0);
    break;
    case Transfer: reg = getNibble(bytes[1], 1);
    break;
    default: reg = -1;
  }
  return reg;
}

/* 
Gets the first operand register from the instruction.
precondition: big-endian word
postcondition: r = Rn or r = 255 (if branching or halting)
*/
int8_t getFirstOperandRegister(int8_t bytes[]) {
  enum InstType type = getInstType(bytes);
  int8_t reg;
  switch(type) {
    case Data: reg = getNibble(bytes[2], 0);
    break;
    case Mul: reg = getNibble(bytes[1], 1);
    break;
    case Transfer: reg = getNibble(bytes[2], 0);
    break;
    default: reg = -1;
  }
  return reg;
}

/* 
Gets the second operand register from the instruction.
precondition: big-endian word 
postcondition: r = Rm or r = 255 (if branching or halting or I flag)
*/
int8_t getSecondOperandRegister(int8_t bytes[]) {
  int32_t word = appendBytes(bytes, 4, true);
  enum InstType type = getInstType(bytes);
  int8_t reg;
  switch (type) {
    case Data: 
      if (isIFlagSet(word)) {
        reg = -1;
      } else { 
        reg = getNibble(bytes[0], 0);
      };
    break;
    case Mul: reg = getNibble(word, 0);
    break;
    case Transfer: 
      if (isIFlagSet(word)) {
        reg = getNibble(bytes[0], 0);
      } else {
        reg = -1;
      };
    default: reg = -1;
  }
  return reg;
}

/* Gets the offset from the big-endian word (not completely done since it can be 
implemented a step further, by calculating the rotate right of the offset - 
will be implemented later)
precondition: big-endian word
postcondition: r = offset
*/
int32_t getOffset(int8_t bytes[]) {
  int32_t word = appendBytes(bytes, 4, true);
  enum InstType type = getInstType(bytes);
  int32_t offset = 0;
  switch(type) {
    case Data: 
      if (isIFlagSet(word)) {
        offset = rotateRight(bytes[0], getNibble(bytes[1], 0) << 1);
      } else { 
        offset = 0;
      };
    break;
    case Transfer: offset = bytes[0] + pow(2,8) * getNibble(bytes[1], 0);
    break;
    case Branch: 
      for (int i = 0; i < 3; i++) {
        offset += pow(2, 8*i) * bytes[i];
      };
    break;
    default: offset = 0;
  }
  return offset;
}

/* 
Gets Register Rs from the instruction set.
Precondition: Big-endian word
Postcondition: r = Rs or r = 255 (if does not exist)
*/

int8_t getRegisterS(int8_t bytes[]) {
  enum InstType type = getInstType(bytes);
  if (type == Mul) {
    return getNibble(bytes[1], 0);
  }
  return -1;
}

/* 
Rotates right a specified value by the rotateAmount:
input:               01110 
output(rotate by 4): 11100
precondition: rotateAmount must be even, and in range of 0-30
postcondition: r = value shifted
*/
int32_t rotateRight(int32_t value, int rotateAmount) {
  assert(rotateAmount % 2 == 0 && rotateAmount >= 0 && rotateAmount <= 30);
  uint32_t temp = (uint32_t) value;
  int32_t mask = 1;
  int32_t apply = 1 << 31;
  for (int i = 0; i < rotateAmount; i++) {
    temp = temp >> 1 | ((temp & mask) == 1 ? apply : 0); 
  }
  return temp;
}

/*
shifts right, perserving the sign bit for 2's complement 
input:               01110000
output(shift by 4):  00000111
precondition: true
postcondition: r = value shifted
*/
int32_t arithmeticShiftRight(int32_t value, int shiftAmount) {
  uint32_t temp = (uint32_t) value;
  int mask = 1 << 31;
  int apply = value & mask;
  for (int i = 0; i < shiftAmount; i++) {
    temp = (temp >> 1) | apply;
  }
  return temp;
}



