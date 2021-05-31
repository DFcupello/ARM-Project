#include "symtable.h"

/*
	Creates initial empty symbol table
	Returns the pointer to its struct
*/
symbolTable_t *allocateInitialSymbolTable(void) {
	
	symbolTable_t * emptyTable = malloc(sizeof(symbolTable_t));
	emptyTable->head = NULL;
	emptyTable->rear = NULL;
	return emptyTable;
}

/*
	Takes label string and 32-bit integer address.
	Returns pointer to the created symbol table node.
	PRE: label ends with '\0' character.
	Function allocates memory for table node and for string which is copied.
*/
symbolEntry_t *allocateNewEntry(char *label, uint32_t addr) {
	
	int labelLength = strlen(label);
	char *labelPtr = malloc(sizeof(char) * (labelLength + 1));
	for (int i = 0; i < labelLength; i++) {
		labelPtr[i] = label[i];
	}
	labelPtr[labelLength] = '\0';

	symbolEntry_t *entry = malloc(sizeof(symbolEntry_t));
	entry->labelPtr = labelPtr;
	entry->address = addr;
	entry->next = NULL;

	return entry;
}


/*
	Takes the pointer to the table, where to add new entry,
	and entry details: label string and 32-bit int address.
	Modifies the table by adding the entry to the REAR.
*/
void addNewEntryToSymbolTable(symbolTable_t *table, char *label, uint32_t addr) {
	
	symbolEntry_t *newEntry = allocateNewEntry(label, addr);
	
	if (table->head == NULL) { // Empty symbol table case
		assert(table->rear == NULL);
		table->head = table->rear = newEntry;
		return;
	}
	// Non-empty symbol table case
	(table->rear)->next = newEntry;
	table->rear = newEntry;
}

/*
	Prints the entry contents in the form ("label", "0xaddress");
*/
void printSymbolTableEntry(symbolEntry_t *entry) {
	printf("(%s, 0x%08x)\n", entry->labelPtr, entry->address);
}

/*
	Prints the table
*/
void printSymbolTable(symbolTable_t *table) {
	printf("(label, address)\n");
	symbolEntry_t *current = table->head;
	if (current == NULL) {
		printf("table is empty\n");
		return;
	}
	while (current != NULL) {
		printSymbolTableEntry(current);
		current = current->next;
	}
}

/*
	Takes the pointer to the table to take entry from.
	Returns NULL if the table is empty, otherwise the pointer to the head.
	Once the Entry is taken, it should be freed by the user, with the aid of freeSymbolTableEntry function
	it will not be freed by the freeSymbolTable function.
*/
symbolEntry_t *takeSymbolTableHead(symbolTable_t *table) {
	if (table->head == NULL) {
		return NULL;
	}
	symbolEntry_t *res = table->head;
	
	if (table->head == table->rear) {
		table->head = table->rear = NULL;
	} else {
		table->head = (table->head)->next;
	}
	return res;
}

/*
	Searches the table for the label, and gets the corresponding address. Returns 0 if not found.
*/
uint32_t getAddress(char *label, symbolTable_t *table) {
	symbolEntry_t *curr = NULL;
	for (curr = table->head; curr && strcmp(label, curr->labelPtr); curr = curr->next);
	if (curr != NULL) {
		return curr->address;
	} 
	return 0;
}

/*
	Takes pointer to the Symbol table node.
	Frees entry label string and then the entry itself.
*/
void freeSymbolTableEntry(symbolEntry_t *entry) {
	free(entry->labelPtr);
	free(entry);
}

/*
	Takes the pointer to the symbol table.
	Frees every node which is still stored in the table and then frees the table itself
*/
void freeSymbolTable(symbolTable_t *table) {
	symbolEntry_t *current = table->head;
	while (current != NULL) {
		symbolEntry_t *entryToFree = current;
		current = current->next;
		freeSymbolTableEntry(entryToFree);
	}
	free(table);
}