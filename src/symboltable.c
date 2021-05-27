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

//Adds label and address to SymbolItem in the given index
void addItem(int index, char *label, int size, uint32_t address, SymbolItem **symbolTable) {
  strncpy((*symbolTable)[index].label, label, size);
  (*symbolTable)[index].label[size] = '\0';
  (*symbolTable)[index].address = address;
}

// Pre-condition: Symbol Table contains label
uint32_t getAddress(char *label, SymbolItem *symbolTable) {
	for (int i = 0; i < STARTING_SIZE; i++) {
		if (strcmp(label, (symbolTable)[i].label) == 0) {
			return (symbolTable)[i].address;
		}
	}
	assert(0);
}

void freeSymbolTable(SymbolItem **symbolTable);

void printTable(int index, SymbolItem **symbolTable) {
  for (int i = 0; i < index; i++) {
	  printf("Label Name: %10s, 0x%08x\n", (*symbolTable)[i].label, (*symbolTable)[i].address);
  }
}


