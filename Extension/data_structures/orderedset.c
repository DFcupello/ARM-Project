#include "orderedset.h"
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
orderedSet *allocateSet(void)
{
	orderedSet *emptyCollection = malloc(sizeof(orderedSet));
	emptyCollection->head = NULL;
	emptyCollection->rear = NULL;
	return emptyCollection;
}

/*
	Takes the pointer to the collection, where to add new node, and node int 
	Modifies the collection by adding it in a way where it would stay ordered.
	Returns false if duplicate.
*/
bool addNode(orderedSet *collection, int val)
{
	setNode *newNode = malloc(sizeof(setNode));
	newNode->value = val;
	newNode->next = NULL;

    if (collection->head == NULL) { // Empty collection case
		assert(collection->rear == NULL);
		collection->head = collection->rear = newNode;
		return true;
	}
	// Non-empty collection case
    setNode *current = collection->head;
    setNode *previous = NULL;
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
bool isEmpty(orderedSet *collection) {
	return (collection->head == NULL);
}

/*
	Prints the node contents in the form (value);
*/
void printNode(setNode *node) {
	printf("(%d)", node->value);
}

/*
	Prints the collection
*/
void printSet(orderedSet *collection) {
	setNode *current = collection->head;
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
Tries to remove a node, specified by the int val. Returns false if not found.
*/
bool removeNode(orderedSet *collection, int val) {
	if (collection->head == NULL) {
		return false;
	}
	if (collection->head == collection->rear && collection->head->value == val) {
		setNode *current = collection->head;
		collection->head = collection->rear = NULL;
		free(current);
		return true;
	} 
	setNode *current = collection->head;
	setNode *previous = NULL;
	while (current && current->value != val) {
		previous = current;
		current = current->next;
	}
	if (current == collection->rear) {
		previous->next = NULL;
		collection->rear = previous;
		free(current);
		return true;
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
bool contains(orderedSet *collection, int value) {
    setNode *current = collection->head;
    for (;current && current->value < value; current = current->next);
    return (current != NULL) && current->value == value;
}

/*
	Takes the pointer to the collection.
	Frees every node which is still stored in the collection and then frees the collection itself
*/
void freeSet(orderedSet *collection)
{
	setNode *current = collection->head;
	while (current != NULL)
	{
		setNode *nodeToFree = current;
		current = current->next;
		free(nodeToFree);
	}
	free(collection);
}

/*
Gets the number of nodes in the set
*/
int getSize(orderedSet *collection) {
	int count = 0;
	for (setNode *current = collection->head; current; current = current->next) {
		count++;
	}
	return count;
}
