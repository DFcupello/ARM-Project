#ifndef OSET
#define OSET
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


/*
	Single node of a simple Linked list based FIFO structure for 32-bit integers
	Contains 32-bit integer value and a pointer to the next node;
*/
typedef struct setNode
{
	int value;
	struct setNode *next;
} setNode;

/*
	Linked list based implementation of FIFO structure for 32-bit integers
	Contains the pointers to the first node and to the last node.
	The second attribute allows adding elements faster.
*/
typedef struct orderedSet
{
	struct setNode *head;
	struct setNode *rear;
} orderedSet;

// priorityQueue Functions
orderedSet *allocateSet(void);
bool addNode(orderedSet *collection, int val);
bool isEmpty(orderedSet *collection);
void printNode(setNode *node);
void printSet(orderedSet *collection);
int popMin(orderedSet *collection);
bool contains(orderedSet *collection, int val);
void freeSet(orderedSet *collection);
int getSize(orderedSet *collection);
bool removeNode(orderedSet *collection, int val);

#endif