#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "symboltable.h"

#define MAX_LINE_LENGTH 511
#define STARTING_SIZE 20
#define INSTRUCTION_COUNT 12

void doFirstPass(FILE *fptr, int *nextAddress, SymbolItem *symbolTable);
void writeBinFile(FILE *binOut, uint32_t *instructions, int size);

int main(int argc, char **argv) {
  int nextAddress = 0;
  char *sourceFile;
  char *outBinFile;
  FILE *fptr = NULL;
  FILE *binOut = NULL;

  // Loads the sourcefile and opens it in reading mode.  
  if (argv[1] != NULL) {
    sourceFile = argv[1];
    fptr = fopen(sourceFile, "r");
  }
  // Creates the binary output file and prepares it in write binary mode.
  if (argv[2] != NULL) {
    outBinFile = argv[2];
    binOut = fopen(outBinFile, "wb");
  }

  
  if (fptr != NULL && binOut != NULL) {

    SymbolItem symbolTable[STARTING_SIZE]; 
    doFirstPass(fptr, &nextAddress, symbolTable);
    fclose(fptr);

    uint32_t binInstr[INSTRUCTION_COUNT]; // Assuming we get this from Diego and Andrey
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
      binInstr[i] = i;
    }
    writeBinFile(binOut, binInstr, INSTRUCTION_COUNT);
    fclose(binOut);
  
    // Second pass, reads opcode mnemonic and operand field and generates the corresponding binary encoding.

  }
  return EXIT_SUCCESS;
}

/*
Loops through assemble file and adds labels with the addresses they point to, to the symbol table
*/
void doFirstPass(FILE *fptr, int *nextAddress, SymbolItem *symbolTable) {
  char currLine[MAX_LINE_LENGTH];
  int labelCount = 0;
  while (fgets(currLine, MAX_LINE_LENGTH, fptr) != NULL) {
    int lineSize = 0;
    while (currLine[lineSize] != '\n' && currLine[lineSize] != 32) {
     lineSize++;
    }
    if (isalpha(currLine[0]) && currLine[lineSize - 1] == ':') { // isalpha returns true if char is a-z or A-Z
      char labelName[MAX_LINE_LENGTH];
      strncpy(labelName, currLine, lineSize - 1);
      labelName[lineSize - 1] = '\0';
      addItem(labelCount, labelName, lineSize - 1, *nextAddress, &symbolTable);
      labelCount++;
    } else {
      *nextAddress = *nextAddress + 4;
    }
  }
  printTable(labelCount, &symbolTable);
}

// Writes array of 32 bit instructions into the binary file given.
void writeBinFile(FILE *binOut, uint32_t *instructions, int size) {
  fwrite(instructions, sizeof(*instructions), size, binOut);
}