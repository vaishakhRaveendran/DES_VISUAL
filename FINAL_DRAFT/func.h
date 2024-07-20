#ifndef FUNC_H
#define FUNC_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "graph.h" 

// Function prototypes
State* createState(int id,bool marked,int maxTransitions);
void addTransition(State* state,int targetState,char label);
void writeDESToFile(State** des, int numStates, const char* filename);
DESResult readDESFromFile(const char* filename);

#endif
