#ifndef TRIM_H
#define TRIM_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "graph.h" 
#include "func.h"
#include "product.h"
#include "parallel.h"


// Function prototypes
void  trim(State** des, int* numStates);
bool coaccessibility(State** des, int numStates, int startStateId);
void deleteState(State** des, int stateId, int* numStates);
#endif