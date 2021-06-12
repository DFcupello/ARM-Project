#ifndef PRIORITY
#define PRIORITY
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
typedef struct priorityNode
{
	int value;
	struct priorityNode *next;
} node;

/*
	Linked list based implementation of FIFO structure for 32-bit integers
	Contains the pointers to the first node and to the last node.
	The second attribute allows adding elements faster.
*/
typedef struct prioQueue
{
	struct priorityNode *head;
	struct priorityNode *rear;
} priorityQueue;

// 32-bit integer fifo Functions
priorityQueue *allocatePriorityQueue(void);
void addNode(priorityQueue *collection, int val);
bool isEmpty(priorityQueue *collection);
void printNode(node *node);
void printPriorityQueue(priorityQueue *collection);
int popMin(priorityQueue *collection);
bool contains(priorityQueue *collection, int val);
void freePriorityQueue(priorityQueue *collection);

#endif