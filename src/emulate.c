#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#define MEM_SIZE 65536
#define REG_SIZE 17

uint32_t data[MEM_SIZE];
uint8_t registers[REG_SIZE];
enum InstType {Data, Mul, Branch, Transfer, HALT};

void binaryLoader(FILE *fptr, char *file, bool endian);
uint8_t getByte(uint32_t word, int n);
uint8_t getNibble(uint32_t word, int n);
uint8_t get2Bits(uint32_t word, int n);
uint8_t getDestinationRegister(uint32_t word);
uint8_t getFirstOperandRegister(uint32_t word);
enum InstType getInstType(uint32_t word);

int main(int argc, char *argv[]) {
  char *file;
  FILE *fptr = NULL;
  if (argv[1] != NULL) {
    file = argv[1];
    fptr = fopen(file, "rb");
  }
  if (fptr != NULL) {
    binaryLoader(fptr, file, true);
    for (int i = 0; i < 10; i++) {
      printf("Rn: %d ", getFirstOperandRegister(data[i]));
      printf("Rdest: %d ", getDestinationRegister(data[i]));
      printf("Type: %d", getInstType(data[i]));
      printf("\n");
    }
    return EXIT_SUCCESS;
  }
  printf("Empty!\n");
  return 0;
}

/* 
Takes a binary file pointer, file, and the bool endian, which if 1,
stores the binary file in arm memory in uint32_t format,
in little endian, and big endian if 0.
*/
void binaryLoader(FILE *fptr, char *file, bool endian) {
  unsigned char buffer[4];
  int i = 0;
  while(i < MEM_SIZE) {
    fread(buffer, sizeof(buffer), 1, fptr);
    if (feof(fptr)) {
      data[i] = 0;
      break;
    }
    uint32_t word = 0;
    for (int j = 0; j < 4; j++) {
      word = word + pow(2, 8 * (endian ? (3 - j) : j)) * buffer[j];
    }
    data[i] = word;
    i++;
  }
  fclose(fptr);
}

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
Must be little endian.
*/
enum InstType getInstType(uint32_t word) {
  uint8_t bits = get2Bits(word, 1);
  assert (bits >= 0 && bits <= 2);
  if (bits == 2) {
    return Branch;
  }
  else if (bits == 1) {
    return Transfer;
  }
  else if (bits == 0) {
    if (getNibble(word, 7) == 9 && getNibble(word, 0) == 0 && get2Bits(word, 7) == 0) {
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
precondition: true
postcondition: r = Rdest or r = 255 (if branching or halting)
*/
uint8_t getDestinationRegister(uint32_t word) {
  enum InstType type = getInstType(word);
  uint8_t reg;
  switch (type) {
    case Data: reg = getNibble(word, 5);
    break;
    case Mul: reg = getNibble(word, 2);
    break;
    case Transfer: reg = getNibble(word, 5);
    break;
    default: reg = 255;
  }
  return reg;
}

/* 
Gets the first operand register from the instruction.
precondition: true
postcondition: r = Rn or r = 255 (if branching or halting)
*/
uint8_t getFirstOperandRegister(uint32_t word) {
  enum InstType type = getInstType(word);
  uint8_t reg;
  switch(type) {
    case Data: reg = getNibble(word, 2);
    break;
    case Mul: reg = getNibble(word, 5);
    break;
    case Transfer: reg = getNibble(word, 2);
    break;
    default: reg = 255;
  }
  return reg;
}













