#ifndef PARALLEL_H
#define PARALLEL_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "graph.h" 
#include "func.h"
#include "product.h"

typedef struct {
    char *commonLabels;
    int commonCount;
    char *exclusiveLabels1;
    int exclusiveCount1;
    char *exclusiveLabels2;
    int exclusiveCount2;
} eventResult;

// Function prototypes
State** parallelComposition();
int check_marked(State* s1, State* s2);
#endif