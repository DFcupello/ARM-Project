#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#include "inhandler.h"
#include "utilities.h"

/* 
The size of memory in 32-bit format, and the number of registers.
*/
#define MEM_SIZE 16384
#define REG_SIZE 17

/*
Address is calculated, since it is byte-addressible, as multiples of 4. So
if we are trying to access data[i], the address is i*4. 
least significant byte (LSB) of data[i] : address  = i*4
most significant byte (MSB) of data[i]  : address  = i*4 + 3
It is in little endian format.
*/

int8_t data[MEM_SIZE][4];
int32_t registers[REG_SIZE];

void binaryLoader(FILE *fptr, char *file, bool bigEndian);
void executeMultiplyInstruction(int8_t bytes[]);

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
      printf("%d. ", i);
      printf("Rn: %d ", getFirstOperandRegister(data[i]));
      printf("Rdest: %d ", getDestinationRegister(data[i]));
      printf("Rm: %d ", getSecondOperandRegister(data[i])); 
      printf("Type: %d ", getInstType(data[i]));
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
in big endian if 1, and little endian if 0.
*/
void binaryLoader(FILE *fptr, char *file, bool bigEndian) {
  unsigned char buffer[4];
  int i = 0;
  while(i < MEM_SIZE) { // This is to ensure it terminates
    fread(buffer, sizeof(buffer), 1, fptr);
    if (feof(fptr)) {
      for (int j = 0; j < 4; j++) {
        data[i][j] = 0;
      }
      break;
    }
    for (int j = 0; j < 4; j++) {
      data[i][bigEndian ? j : (3 - j)] = buffer[j];
    }
    i++;
  }
  fclose(fptr);
}

/* 
parses the multiply instruction and executes the instruction
If A flag set:
  Rdest = Rm * Rs + Rn
If not:
  Rdest = Rm * Rs
*/
void executeMultiplyInstruction(int8_t bytes[]) {
  assert(getInstType(bytes) == Mul);
  int32_t word    = appendBytes(bytes, 4, true);
  int32_t regDest = (int32_t) getDestinationRegister(bytes);
  int32_t regN    = isAFlagSet(word) ? registers[getFirstOperandRegister(bytes)] : 0;
  int32_t regM    = registers[getSecondOperandRegister(bytes)];
  int32_t regS    = registers[getRegisterS(bytes)];

  if (isSFlagSet(word)) { // Need to change CSPR register (TODO)
    registers[regDest] = regM * regS + regN;
  }
  else {
    registers[regDest] = regM * regS + regN;
  }
}



