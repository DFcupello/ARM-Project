#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "symboltable.h"

#define STARTING_SIZE 20

void addItem(int index, char *label, uint32_t address, SymbolItem **symbolTable) {
    (*symbolTable)[index].label = label;
    (*symbolTable)[index].address = address;
	printf("Label Name: %10s, 0x%08x\n", (*symbolTable)[index].label, (*symbolTable)[index].address);
}

void getItem(char *label, SymbolItem **symbolTable);

void freeSymbolTable(SymbolItem **symbolTable);

void printTable(int index, SymbolItem **symbolTable) {
	for (int i = 0; i < index; i++) {
		printf("Label Name: %10s, 0x%08x\n", (*symbolTable)[i].label, (*symbolTable)[i].address);
	}
}


