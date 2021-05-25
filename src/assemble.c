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

void doFirstPass(FILE *fptr, int *nextAddress, map *symbolTable);
/*
typedef struct Label {
  char *label;
  uint32_t address;
} Label_struct;
*/

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

    //Label_struct symbolTable[10]; //I don't know how big we should make this array, will leave at 10 for now.
    map symbolTable = map_nil();
    doFirstPass(fptr, &nextAddress, &symbolTable);
    fclose(fptr);
    char* name = "test.txt";
    FILE *fptr2 = fopen(name, "w");
    //print_map(fptr2,symbolTable);
    //fclose(fptr2);
    print_map2(symbolTable);
    
  }

  // Second pass, reads opcode mnemonic and operand field and generates the corresponding binary encoding.

  return EXIT_SUCCESS;
}
void doFirstPass(FILE *fptr, int *nextAddress, map *symbolTable) {
  char currLine[MAX_LINE_LENGTH];
  while (fgets(currLine, MAX_LINE_LENGTH, fptr) != NULL) {
    *nextAddress = *nextAddress + 4;
    int i = 0;
    while (currLine[i] != '\n' && currLine[i] != 32) {
     i++;
    }
    int lineSize = i;
    if (isalpha(currLine[0]) && currLine[lineSize - 1] == ':') { 
      *nextAddress = *nextAddress - 4;
      char labelName[MAX_LINE_LENGTH];
      strncpy(labelName, currLine, lineSize - 1);
      labelName[lineSize - 1] = '\0';
      *symbolTable = map_cons(labelName, *nextAddress, *symbolTable);
    }
  }
}