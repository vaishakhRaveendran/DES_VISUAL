#ifndef SYNTHESISER_H
#define SYNTHESISER_H

#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "graph.h" 
#include "func.h"
#include "product.h"
#include "parallel.h"

#define MAX_SYN_STACK_SIZE 100

//Structures
typedef struct {
    int items[MAX_SYN_STACK_SIZE];
    int top;
} SynStack;

// Function prototypes
void initializeSynStack(SynStack *stack);
int isSynStackEmpty(SynStack *stack);
int isSynStackFull(SynStack *stack);
void pushToSynStack(SynStack *stack, int item);
int popFromSynStack(SynStack *stack);
bool isPresent(int arr[], int index, int num);
int* findUnion(int *arr1, int size1, int *arr2, int size2, int *unionSize);
bool reachForbidden(State **des, int numStates, int startStateId, int not_Q_prime[], int len_notQPrime);
bool syn_coaccessibility(State** des, int numStates, int startStateId, int forbiddenStates[], int* len_forbiddenStates);
int* restrictedBackwards(State** des, int numStates, int forbiddenStates[], int* len_Q_prime, int* len_forbiddenStates);
int* getStatesNotInQPrime(int numStates, int* Q_prime, int len_Q_prime, int* len_notQPrime);
int* uncontrolledBackwards(State** des, int numStates, int *len_Q_double_prime, int notQPrime[], int *len_notQPrime);
void syn_removeStates(State** des, int stateId, int* numStates);
State** restrictedForward(State** des, int* numStates, int startStateId, int forbiddenStates[], int* len_forbiddenStates);
int findStateIndex(State** des, int numStates, int stateId);
void update_forbidden_list(int forbiddenStates[], int* len_forbidden, int index);
int safe_state_synthesiser();

#endif