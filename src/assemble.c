#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "symboltable.h"
#include "inassembler.h"
#include "inhandler.h"

#define MAX_LINE_LENGTH 511
#define STARTING_SIZE 20
#define INSTRUCTION_COUNT 12

void doFirstPass(FILE *fptr, int *nextAddress, SymbolItem *symbolTable, int *numOfInstrs);
void writeBinFile(FILE *binOut, uint32_t *instructions, int size);
void doSecondPass(FILE *fptr, SymbolItem *symbolTable, uint32_t *instructions, int *numOfLines);

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
    int numOfInstrs = 0;
    doFirstPass(fptr, &nextAddress, symbolTable, &numOfInstrs);
    uint32_t instructions[numOfInstrs];
    // Second pass, reads opcode mnemonic and operand field and generates the corresponding binary encoding.
    rewind(fptr);
    int numOfLines = numOfInstrs;
    doSecondPass(fptr, symbolTable, instructions, &numOfLines);
    writeBinFile(binOut, instructions, numOfInstrs);
    fclose(binOut);
    fclose(fptr);
  }
  return EXIT_SUCCESS;
}

/*
Loops through assemble file and adds labels with the addresses they point to, to the symbol table
*/
void doFirstPass(FILE *fptr, int *nextAddress, SymbolItem *symbolTable, int *numOfInstrs) {
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
      if (currLine[0] != '\n') {
        *nextAddress = *nextAddress + 4;
        (*numOfInstrs)++;
      }
    }
  }
  //printTable(labelCount, &symbolTable);
}

// Writes array of 32 bit instructions into the binary file given.
void writeBinFile(FILE *binOut, uint32_t *instructions, int size) {
  fwrite(instructions, sizeof(*instructions), size, binOut);
}

void doSecondPass(FILE *fptr, SymbolItem *symbolTable, uint32_t *instructions, int *numOfLines)
{
  char currLine[MAX_LINE_LENGTH];
  int instrCount = 0;
  while (fgets(currLine, MAX_LINE_LENGTH, fptr) != NULL) {
    int lineSize = 0;
    bool isInstr = false;
    while (currLine[lineSize] != '\0') {
      if (currLine[0] == '\n') {
        break;
      }
      if (currLine[lineSize] == 32) {
        isInstr = true;
        break;
      }
      lineSize++;
    }
    if (isInstr) { 
      uint32_t currAddress = instrCount * 4;
      instructions[instrCount] = assembleInstruction(currLine, symbolTable, numOfLines, currAddress);
      instrCount++;
    }
  }
}