#ifndef SLLIST_H_INCLUDED
#define SLLIST_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//define the Node
typedef struct node
{
    struct node *next;
    int content;
} Node;

//define the SLList
typedef struct
{
    Node *first;
    Node *last;
} SLList;

//define the functions that initialize a List
void initList(SLList *List);

// define the 4 necessary operations
void insertAtRear(SLList *List, int x);

void deleteAtFront(SLList *List);

void insertAtFront(SLList *List, int x);

void deleteKey(SLList *List, int index);

int contentAt(SLList* List, int index);

void insertAt(SLList* List, int index, int content);

//define the functions needed to print the stacks and queues
void printList(SLList *List);

Node* nodeAT(SLList* List, int index);

void swapNodes(SLList* List, int x, int y);


#endif // SLLIST_H_INCLUDED
