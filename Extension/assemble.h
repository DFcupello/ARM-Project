#ifndef ASSEMBLE
#define ASSEMBLE

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include "fifos.h"
#include "symtable.h"

#define MAX_LINE_LENGTH 511
#define STARTING_SIZE 20
#define INSTRUCTION_COUNT 12

void doFirstPass(FILE *fptr, int *nextAddress, symbolTable_t *symbolTable, int *numOfInstrs);
void writeBinFile(FILE *binOut, uint32_t *instructions, int size, ldrCollection_t *queue, int extralines);
void doSecondPass(FILE *fptr, symbolTable_t *symbolTable, uint32_t *instructions, int *numOfLines, ldrCollection_t *queue);
void assemble(FILE *fptr, FILE *binOut);


#endif