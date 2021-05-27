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

void doFirstPass(FILE *fptr, int *nextAddress, SymbolItem *symbolTable);

int main(int argc, char **argv) {
  int nextAddress = 0;
  char *sourceFile;
  char *outBinFile;
  FILE *fptr = NULL;
  FILE *binOut = NULL;
  //if (argv[1] != NULL) {
    sourceFile = "/homes/nm920/C/arm11_testsuite/test_cases/loop02.s";
    fptr = fopen(sourceFile, "r");
  //}
  if (argv[2] != NULL) {
    outBinFile = argv[2];
    binOut = fopen(outBinFile, "wb");
  }
  if (fptr != NULL) {
    // Can begin the first pass, creating a symbol table (labels (char[]) -> memory adresses (uint32_t[]))).
    // Similar to binaryLoader in emulate we can can read the sourceFile line by line and fill the label and adress arrays

    //Perhaps create a map like struct? Or we could have just an array of labels and 
    //corresponding array containg arrays of addresses?

    SymbolItem symbolTable[STARTING_SIZE]; 
    doFirstPass(fptr, &nextAddress, symbolTable);
    fclose(fptr);
    printf("0x%08x\n", getAddress("wait", symbolTable));
    char* name = "test.txt";
    FILE *fptr2 = fopen(name, "w");
    //print_map(fptr2,symbolTable);
    //fclose(fptr2);

  }

  // Second pass, reads opcode mnemonic and operand field and generates the corresponding binary encoding.

  return EXIT_SUCCESS;
}
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