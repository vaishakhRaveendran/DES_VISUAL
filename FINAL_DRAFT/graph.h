#ifndef GRAPH_H_
#define GRAPH_H_

#include<stdbool.h>

//A TRANSITION IDENTIFIED USING START STATE,NEXT STATE AND LABEL.
typedef struct Transition{
   int targetState;
   char label;
}Transition;

//STATE IDENTIFIED BY ID,MAX-NO-TRANSITIONS AND ITS TRANSITIONS.
typedef struct State{
   int id;
   int marked;
   Transition *transitions;
   int numTransitions;
   int maxTransitions;
}State;

typedef struct {
    State** des;
    int numStates;
} DESResult;

void generateGraph(State** des, int numStates);

#endif /* GRAPH_H_ */
