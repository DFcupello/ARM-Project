#include "priorityqueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/*
	Creates initial empty collection
	Returns the pointer to its struct
*/
priorityQueue *allocatePriorityQueue(void)
{
	priorityQueue *emptyCollection = malloc(sizeof(priorityQueue));
	emptyCollection->head = NULL;
	emptyCollection->rear = NULL;
	return emptyCollection;
}

/*
	Takes the pointer to the collection, where to add new node, and node 32-bit int address.
	Modifies the collection by adding the node to the REAR.
*/
bool addNode(priorityQueue *collection, int val)
{
	node *newNode = malloc(sizeof(node));
	newNode->value = val;
	newNode->next = NULL;

    if (collection->head == NULL) { // Empty collection case
		assert(collection->rear == NULL);
		collection->head = collection->rear = newNode;
		return true;
	}
	// Non-empty collection case
    node *current = collection->head;
    node *previous = NULL;
    while (current && newNode->value > current->value) {
        previous = current;
		current = current->next;
    }
	if (current != NULL && current->value == val) {
		free(newNode);
		return false;
	}
	if (previous == NULL) { // head case
        newNode->next = current;
        collection->head = newNode;
        return true;
    }
    if (current == NULL) { // rear case 
        (collection->rear)->next = newNode;
	    collection->rear = newNode;
        return true;
	}
    previous->next = newNode;
    newNode->next = current;
	return true;
}

/*
	Returns true if queue is empty
*/
bool isEmpty(priorityQueue *collection) {
	return (collection->head == NULL);
}

/*
	Prints the node contents in the form (value);
*/
void printNode(node *node) {
	printf("(%d)", node->value);
}

/*
	Prints the collection
*/
void printPriorityQueue(priorityQueue *collection) {
	node *current = collection->head;
	if (current == NULL)
	{
		printf("collection is empty\n");
		return;
	}
	while (current != NULL)
	{
		printNode(current);
		current = current->next;
	}
	printf("\n");
}


/*
	Takes the pointer to the collection to take the value from.
	Returns NULL the value of the head and then frees that head
	PRE: collection is non empty
*/
int popMin(priorityQueue *collection)
{
	assert(collection->head != NULL);
	node *oldHead = collection->head;
	int res = oldHead->value;

	if (collection->head == collection->rear)
	{
		collection->head = collection->rear = NULL;
	}
	else
	{
		collection->head = (collection->head)->next;
	}
	free(oldHead);
	return res;
}

bool removeNode(priorityQueue *collection, int val) {
	if (collection->head == NULL) {
		return false;
	}
	if (collection->head == collection->rear && collection->head->value == val) {
		collection->head = collection->rear = NULL;
		return true;
	}
	node *current = collection->head;
	node *previous = NULL;
	while (current && current->value != val) {
		previous = current;
		current = current->next;
	}
	if (current == NULL) { // NOT FOUND
		return false;
	}
	if (previous == NULL) { // current is head
		collection->head = current->next;
		free(current);
		return true;
	}
	previous->next = current->next;
	free(current);
	return true;
}

/*
    Returns true if found.
*/
bool contains(priorityQueue *collection, int value) {
    node *current = collection->head;
    for (;current && current->value < value; current = current->next);
    return (current != NULL) && current->value == value;
}

/*
	Takes the pointer to the collection.
	Frees every node which is still stored in the collection and then frees the collection itself
*/
void freePriorityQueue(priorityQueue *collection)
{
	node *current = collection->head;
	while (current != NULL)
	{
		node *nodeToFree = current;
		current = current->next;
		free(nodeToFree);
	}
	free(collection);
}

/*
Gets the number of nodes in a priority queue
*/
int getSize(priorityQueue *collection) {
	int count = 0;
	for (node *current = collection->head; current; current = current->next) {
		count++;
	}
	return count;
}
