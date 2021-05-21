#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#include "inhandler.h"
#include "utilities.h"

#define MEM_SIZE 65536
#define REG_SIZE 17

uint32_t data[MEM_SIZE];
uint32_t registers[REG_SIZE];
void binaryLoader(FILE *fptr, char *file, bool endian);

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
      uint32_t bigWord = littleEndToBigEnd(data[i]);
      printf("Rn: %d ", getFirstOperandRegister(bigWord));
      printf("Rdest: %d ", getDestinationRegister(bigWord));
      printf("Rn: %d", getSecondOperandRegister(bigWord)); 
      printf("Type: %d", getInstType(bigWord));
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
