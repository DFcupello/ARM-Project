#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#define MEM_SIZE 65536
#define REG_SIZE 17

uint32_t data[MEM_SIZE];
uint32_t registers[REG_SIZE];

void binaryLoader(FILE *fptr, char *file);

int main(int argc, char *argv[]) {
  
  char *file;
  FILE *fptr = NULL;
  if (argv[1] != NULL) {
    file = argv[1];
    fptr = fopen(file, "rb");
  }
  if (fptr != NULL) {
    binaryLoader(fptr, file);
    return EXIT_SUCCESS;
  }
  printf("Empty!\n");
  return 0;
}

// Loads the binary file
void binaryLoader(FILE *fptr, char *file) {

  unsigned char c;
  unsigned char buffer[10];
  
  fread(buffer, sizeof(buffer),1, fptr);

  for(int i = 0; i < 10; i++) {
    printf("0x%x ", buffer[i]);
  }

  printf("\n");
  fclose(fptr);
}

// Parse the binary file
void parseBinaryFile(void) {

}

// Distinguish between instructions 
void distinguishInstructions(void) {

}









