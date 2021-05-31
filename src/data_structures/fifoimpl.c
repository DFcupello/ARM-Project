#include "fifos.h"

/*
	Creates initial empty collection
	Returns the pointer to its struct
*/
ldrCollection_t * allocateInitialLdrCollection(void) {
	ldrCollection_t * emptyCollection = malloc(sizeof(ldrCollection_t));
	emptyCollection->head = NULL;
	emptyCollection->rear = NULL;
	return emptyCollection;
}

/*
	Takes the pointer to the collection, where to add new node, and node 32-bit int address.
	Modifies the collection by adding the node to the REAR.
*/
void addNewNodeToLdrCollection(ldrCollection_t *collection, uint32_t val) {
	
	fifoNode_t *newNode = malloc(sizeof(fifoNode_t));
	newNode->value = val;
	newNode->next = NULL;
	
	if (collection->head == NULL) { // Empty collection case
		assert(collection->rear == NULL);
		collection->head = collection->rear = newNode;
		return;
	}
	// Non-empty collection case
	(collection->rear)->next = newNode;
	collection->rear = newNode;
}

/*
	Prints the node contents in the form (0x"value");
*/
void printLdrCollectionNode(fifoNode_t *node) {
	printf("(0x%08x)", node->value);
}

/*
	Prints the collection
*/
void printLdrCollection(ldrCollection_t *collection) {
	fifoNode_t *current = collection->head;
	if (current == NULL) {
		printf("collection is empty\n");
		return;
	}
	while (current != NULL) {
		printLdrCollectionNode(current);
		current = current->next;
	}
	printf("\n");
}

/*
	Takes the pointer to the collection to take the value from.
	Returns NULL the value of the head and then frees that head
	PRE: collection is non empty
*/
uint32_t takeLdrCollectionHeadValue(ldrCollection_t *collection) {
	assert(collection->head != NULL);
	fifoNode_t *oldHead = collection->head;
	uint32_t res = oldHead->value;
	
	if (collection->head == collection->rear) {
		collection->head = collection->rear = NULL;
	} else {
		collection->head = (collection->head)->next;
	}
	free(oldHead);
	return res;
}

/*
	Takes the pointer to the collection.
	Frees every node which is still stored in the collection and then frees the collection itself
*/
void freeldrCollection(ldrCollection_t *collection) {
	fifoNode_t *current = collection->head;
	while (current != NULL) {
		fifoNode_t *nodeToFree = current;
		current = current->next;
		free(nodeToFree);
	}
	free(collection);
}