#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#define MEM_SIZE 65536
#define REG_SIZE 17

uint32_t data[MEM_SIZE];
uint32_t registers[REG_SIZE];
enum inst{Data, Mul, Branch, Transfer};

void binaryLoader(FILE *fptr, char *file, bool endian);
uint32_t getByte(uint32_t word,int byte);
uint32_t getNibble(uint32_t word, int n);

int main(int argc, char *argv[]) {
  
  char *file;
  FILE *fptr = NULL;
  if (argv[1] != NULL) {
    file = argv[1];
    fptr = fopen(file, "rb");
  }
  if (fptr != NULL) {
    binaryLoader(fptr, file, true);
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
    printf("----\n");
    if (feof(fptr)) {
      data[i] = 0;
      break;
    }
    uint32_t word = 0;
    for (int j = 0; j < 4; j++) {
      printf("%d\n", buffer[j]);
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

uint32_t getByte(uint32_t word,int n) {
  assert (n >= 0 && n <=3);  
  uint32_t mask = 255;
  uint32_t answer = 0;
  mask = mask << 8 * n;
  answer = mask & word;
  answer = answer >> 8 * n;
  return answer;
}

/* 
  Gets nth nibble (0 is LSN, 7 is MSN)
  precondition: n is of range 0-7
  postcondition: r = nth nibble
*/

uint32_t getNibble(uint32_t word, int n) {
  assert (n >= 0 && n <= 7);  
  uint32_t mask = 255;
  uint32_t answer = 0;
  mask = mask << 4 * n;
  answer = mask & word;
  answer = answer >> 4 * n;
  return answer;
}

// convert to Binary 
void intToBinary(void) {

}

// Distinguish between instructions 
void distinguishInstructions(void) {

}











