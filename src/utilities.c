
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
Gets nth byte (0 is LSB, 3 is MSB)
precondition: n is of range 0-3
postcondition: r = nth byte
*/

uint8_t getByte(uint32_t word, int n) {
  assert (n >= 0 && n <=3);  
  uint32_t mask = 255;
  uint32_t answer = 0;
  mask = mask << 8 * n;
  answer = mask & word;
  answer = answer >> 8 * n;
  assert (answer >= 0 && answer <= 255);
  return (uint8_t) answer;
}

/* 
  Gets nth nibble (0 is LSN, 7 is MSN)
  precondition: n is of range 0-7
  postcondition: r = nth nibble
*/

uint8_t getNibble(uint32_t word, int n) {
  assert (n >= 0 && n <= 7);  
  uint32_t mask = 15;
  uint32_t answer = 0;
  mask = mask << 4 * n;
  answer = mask & word;
  answer = answer >> 4 * n;
  assert (answer >= 0 && answer <= 15);
  return (uint8_t) answer;
}

/* 
  Gets nth 2-bits (0 is LSb, 7 is MSb)
  precondition: n is of range 0-15
  postcondition: r = nth 2-bit
*/

uint8_t get2Bits(uint32_t word, int n) {
  assert (n >= 0 && n <= 15);  
  uint32_t mask = 3;
  uint32_t answer = 0;
  mask = mask << 2 * n;
  answer = mask & word;
  answer = answer >> 2 * n;
  return (uint8_t) answer;
}

/*
Gets the type of instruction from a given word.
Must be big endian
*/
enum InstType getInstType(uint32_t word) {
  uint8_t bits = get2Bits(word, 13);
  assert (bits >= 0 && bits <= 2);
  if (bits == 2) {
    return Branch;
  }
  else if (bits == 1) {
    return Transfer;
  }
  else if (bits == 0) {
    if (getNibble(word, 1) == 9 && getNibble(word, 6) == 0 && get2Bits(word, 11) == 0) {
      return Mul;
    }
    if (word == 0) {
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
uint8_t getDestinationRegister(uint32_t word) {
  enum InstType type = getInstType(word);
  uint8_t reg;
  switch (type) {
    case Data: reg = getNibble(word, 3);
    break;
    case Mul: reg = getNibble(word, 4);
    break;
    case Transfer: reg = getNibble(word, 3);
    break;
    default: reg = 255;
  }
  return reg;
}

/* 
Gets the first operand register from the instruction.
precondition: big-endian word
postcondition: r = Rn or r = 255 (if branching or halting)
*/
uint8_t getFirstOperandRegister(uint32_t word) {
  enum InstType type = getInstType(word);
  uint8_t reg;
  switch(type) {
    case Data: reg = getNibble(word, 4);
    break;
    case Mul: reg = getNibble(word, 3);
    break;
    case Transfer: reg = getNibble(word, 4);
    break;
    default: reg = 255;
  }
  return reg;
}

/* 
Gets the second operand register from the instruction.
precondition: big-endian word
postcondition: r = Rm or r = 255 (if branching or halting or I flag)
*/
uint8_t getSecondOperandRegister(uint32_t word) {
  enum InstType type = getInstType(word);
  uint8_t reg;
  switch (type) {
    case Data: if (isIFlagSet(word)) {reg = 255;} else { reg = getNibble(word, 0);};
    break;
    case Mul: reg = getNibble(word, 0);
    break;
    case Transfer: if (isIFlagSet(word)) {reg = getNibble(word, 0);} else {reg = 255;};
    default: reg = 255;
  }
  return reg;
}

/* Gets the offset from the big-endian word (not completely done since it can be 
implemented a step further, by calculating the rotate right of the offset - will be implemented 
later)
precondition: big-endian word
postcondition: r = offset
*/
uint32_t getOffset(uint32_t word) {
  enum InstType type = getInstType(word);
  uint32_t offset = 0;

  switch(type) {
    case Data: if (isIFlagSet(word)) {offset = getByte(word, 0);} else { offset = 0;};
    break;
    case Transfer: offset = getByte(word, 0) + pow(2,8) * getNibble(word, 2);
    break;
    case Branch: for (int i = 0; i < 3; i++) {offset += pow(2, 8*i) * getByte(word, i);};
    break;
    default: offset = 0;
  }
  return offset;
}