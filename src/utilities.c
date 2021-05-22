#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include "utilities.h"
#include "inhandler.h"


/* 
Takes a binary file pointer, file, and the bool endian, which if 1,
stores the binary file in arm memory in int32_t format,
in big endian if 1, and little endian if 0.
*/
void binaryLoader(FILE *fptr, char *file, bool bigEndian, int32_t *data, int size) {
  unsigned char buffer[4];
  int i = 0;
  while(i < size) { // This is to ensure it terminates
    fread(buffer, sizeof(buffer), 1, fptr);
    if (feof(fptr)) {
      for (int j = 0; j < 4; j++) {
        data[i] = 0;
      }
      break;
    }
    uint32_t word = 0;
    for (int j = 0; j < 4; j++) {
      word += pow(2, 8 * (bigEndian ? j : (3 - j))) * buffer[j];
    }
    data[i] = word;
    i++;
  }
  fclose(fptr);
}

/*
  Gets n amount of bit in a certain position (including) (0th position is LSb)
  Example: input: (0101 0000 1010 1111 1110 0100 1101 1100, 4, 12) output: 1110
  Example: input: (0101 0000 1010 1111 1110 0100 1101 1100, 7, 0) output: 
  101 1100
*/
int32_t getNBits(int32_t word, int amount, int pos) {
  assert(amount >= 0 && pos >= 0 && amount + pos < 32);
  int32_t mask = pow(2, amount) - 1;
  uint32_t answer = 0;
  mask = mask << pos;
  answer = mask & word;
  answer = answer >> pos;
  return answer;
}

/*
  Appends n bytes together (0 is LSb)
  Example in big Endian: Input: {0101, 1111, 1001} Output: 0000100111110101
*/

/*
int32_t appendBytes(int32_t word, bool bigEndian) {
  uint32_t newWorde = 0;
  for (int i = 0; i < size; i++) {
    newWorde |= ((uint8_t) bytes[bigEndian ? i : ((size - 1) - i)]) << (8 * i);
  }
  return (int32_t) newWorde;
}
*/


/*
Gets the type of instruction from a given word.
Must be big endian.
*/
enum InstType getInstType(int32_t word) {
  int32_t bits = getNBits(word, 2, 26);
  assert (bits >= 0 && bits <= 2);
  if (bits == 2) {
    return Branch;
  }
  else if (bits == 1) {
    return Transfer;
  }
  else if (bits == 0) {
    if (getNBits(word, 4, 4) == 9 && getNBits(word, 4, 24) == 0 && getNBits(word, 2, 22) == 0) {
      return Mul;
    }
    if (word == 0) {
      return HALT;
    }
    return Data;
  }
  assert(0);
  return -1;
}


/*
Gets the destination register from the instruction, assuming it is not branching.
precondition: big-endian word
postcondition: r = Rdest or r = 255 (if branching or halting)
*/
int32_t getDestinationRegister(int32_t word) {
  enum InstType type = getInstType(word);
  int32_t reg;
  switch (type) {
    case Transfer:
    case Data: reg = getNBits(word, 4, 12);
    break;
    case Mul: reg = getNBits(word, 4, 16);
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
int32_t getFirstOperandRegister(int32_t word) {
  enum InstType type = getInstType(word);
  int32_t reg;
  switch(type) {
    case Transfer:
    case Data: reg = getNBits(word, 4, 16);
    break;
    case Mul: reg = getNBits(word, 4, 12);
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
int32_t getSecondOperandRegister(int32_t word) {
  enum InstType type = getInstType(word);
  int32_t reg;
  switch (type) {
    case Data: 
      if (isIFlagSet(word)) {
        reg = -1;
      } else { 
        reg = getNBits(word, 4, 0);
      };
    break;
    case Mul: reg = getNBits(word, 4, 0);
    break;
    case Transfer: 
      if (isIFlagSet(word)) {
        reg = getNBits(word, 4, 0);
      } else {
        reg = -1;
      };
    default: reg = -1;
  }
  return reg;
}

/* Gets the offset from the big-endian word.
precondition: big-endian word
postcondition: r = offset
*/
int32_t getOffset(int32_t word) {
  enum InstType type = getInstType(word);
  assert(type != Mul);
  int32_t offset = 0;
  bool dummy = false;
  switch(type) {
    case Data: offset = rotateRight(getNBits(word, 8, 0), getNBits(word, 4, 8) << 1, &dummy);
    break;
    case Transfer: offset = getNBits(word, 12, 0);
    break;
    case Branch: offset = getNBits(word, 24, 0);
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

int32_t getRegisterS(int32_t word) {
  enum InstType type = getInstType(word);
  if (type == Mul) {
    return getNBits(word, 4, 0);
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
int32_t rotateRight(int32_t value, int rotateAmount, bool *carry) {
  assert(rotateAmount % 2 == 0 && rotateAmount >= 0 && rotateAmount <= 30);
  uint32_t temp = (uint32_t) value;
  int32_t mask = 1;
  int32_t apply = 1 << 31;
  for (int i = 0; i < rotateAmount; i++) {
    if ((temp & mask) == 1) {
      *carry = true;
    }
    temp = (temp >> 1) | ((temp & mask) == 1 ? apply : 0); 
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
int32_t arithmeticShiftRight(int32_t value, int shiftAmount, bool *carry) {
  uint32_t temp = (uint32_t) value;
  int mask = 1 << 31;
  int apply = temp & mask;
  int carryCheck = 1;
  for (int i = 0; i < shiftAmount; i++) {
    if ((carryCheck & temp) == 1) {
      *carry = true;
    }
    temp = (temp >> 1) | apply;
  }
  return temp;
}

/*
shifts right if (right = true), left otherwise
precondition: true
postcondition: r = value shifted
*/
int32_t logicalShift(int32_t value, int shiftAmount, bool right, bool *carry) {
  uint32_t temp = (uint32_t) value;
  int carryCheck = right ? 1 : 1 << 31;
  for (int i = 0; i < shiftAmount; i++) {
    if ((carryCheck & temp) == 1) {
      *carry = true;
    }
    temp = right ? temp >> 1 : temp << 1;
  }
  return temp;
}

/*
Gets the shifted register from the instruction.
Must be transfer/data.
*/

int32_t getShiftedRegister(int32_t word, int32_t registers[], bool *carry) {
  if (getNBits(word, 1, 4) == 0) {
    uint32_t regM = registers[getSecondOperandRegister(word)];
    int32_t integer = getNBits(word, 5, 8);
    int32_t shiftType = getNBits(word, 2, 5);
    int32_t answer;
    switch (shiftType) {
      case 0: answer = logicalShift(regM, integer, false, carry);
      break;
      case 1: answer = logicalShift(regM, integer, false, carry);
      case 2: answer = arithmeticShiftRight(regM, integer, carry);
      case 3: answer = rotateRight(regM, integer, carry);
    }
    return answer;
  }
  return 0;
}



