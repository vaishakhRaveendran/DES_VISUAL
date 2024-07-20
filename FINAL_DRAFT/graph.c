#include<stdio.h>
#include "graph.h"
#include<stdlib.h>

void generateGraph(State** des, int numStates) {
    FILE *fp;
    fp = fopen("graph.dot", "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    fprintf(fp, "digraph G {\n");
    fprintf(fp, "    0 [shape=rectangle];\n");
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < des[i]->numTransitions; j++) {
            fprintf(fp, "%d -> %d [label=\"%c\"];\n", des[i]->id, des[i]->transitions[j].targetState, des[i]->transitions[j].label);
        }
        if (des[i]->marked) {
            fprintf(fp, "%d [shape=doublecircle];\n", des[i]->id);
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);

    system("dot -Tpng graph.dot -o graph.png");
    printf("Graph generated successfully\n");

    
}
