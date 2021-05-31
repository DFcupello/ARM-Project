#ifndef FIFO
#define FIFO
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/*
	Single node of a simple Linked list based FIFO structure for 32-bit integers
	Contains 32-bit integer value and a pointer to the next node;
*/
typedef struct fifoNode {
	uint32_t value;
	struct fifoNode *next;
} fifoNode_t;

/*
	Linked list based implementation of FIFO structure for 32-bit integers
	Contains the pointers to the first node and to the last node.
	The second attribute allows adding elements faster.
*/
typedef struct fifo {
	struct fifoNode *head;
	struct fifoNode *rear;
} ldrCollection_t;

// 32-bit integer fifo Functions
ldrCollection_t *allocateInitialLdrCollection(void);
void addNewNodeToLdrCollection(ldrCollection_t *collection, uint32_t val);
void printLdrCollectionNode(fifoNode_t *node);
void printLdrCollection(ldrCollection_t *collection);
uint32_t takeLdrCollectionHeadValue(ldrCollection_t *collection);
void freeldrCollection(ldrCollection_t *collection);

#endif