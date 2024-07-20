#ifndef PRODUCT_H
#define PRODUCT_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "graph.h" 
#include "func.h"

typedef struct Node {
    int id1;
    int id2;
    int id;
    struct Node* next;
}Node;

//STACK FOR PRODUCT COMPOSITON
typedef struct{
    int id1;
    int id2;
}tuple;

// Structure for stack node
typedef struct StackNode {
    tuple data;
    struct StackNode* next;
} StackNode;

// Structure for stack
typedef struct {
    StackNode* top;
} Stack;

//Structure for common labels.
typedef struct {
    char *labels; 
    int count;
} CommonLabelsResult;


// Function prototypes
State** productComposition();
StackNode* createStackNode(tuple data);
void initializeStack(Stack* stack);
void push(Stack* stack, tuple data);
tuple pop(Stack* stack);
int isEmpty(Stack* stack);
void freeStack(Stack* stack);
Node* createNode(Node* lastNode, int id1, int id2, int id);
int isNodeExists(Node* startPtr, int id1, int id2);
int getFinalStateByLabel(State* state, char label);
#endif
