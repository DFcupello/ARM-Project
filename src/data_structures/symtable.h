#ifndef SYMTABLE
#define SYMTABLE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/*
	Single node of a symbol table containing a pointer to a label string,
	32-bit integer address and a pointer to the next node;
*/
typedef struct linkedListNode {
	char *labelPtr;
	uint32_t address;
	struct linkedListNode *next;
} symbolEntry_t;

/*
	Linked list based implementation of symbol table
	Contains the pointers to the first node and to the last node.
	The second attribute allows adding elements faster.
*/
typedef struct linkedList {
	struct linkedListNode *head;
	struct linkedListNode *rear;
} symbolTable_t;

// Symbol Table Fuctions
symbolTable_t *allocateInitialSymbolTable(void);
symbolEntry_t *allocateNewEntry(char *label, uint32_t addr);
void addNewEntryToSymbolTable(symbolTable_t *table, char *label, uint32_t addr);
void printSymbolTableEntry(symbolEntry_t *entry);
void printSymbolTable(symbolTable_t *table);
symbolEntry_t *takeSymbolTableHead(symbolTable_t *table);
uint32_t getAddress(char *label, symbolTable_t *table);
void freeSymbolTableEntry(symbolEntry_t *entry);
void freeSymbolTable(symbolTable_t *table);

#endif