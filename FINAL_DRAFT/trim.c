#include "trim.h"
#define MAX_STACK_SIZE 100

typedef struct {
    int items[MAX_STACK_SIZE];
    int top;
} CustomStack;

// Initialize stack
void initializeCustomStack(CustomStack *customStack) {
    customStack->top = -1;
}

// Check if stack is empty
int isCustomStackEmpty(CustomStack *customStack) {
    return (customStack->top == -1);
}

// Check if stack is full
int isCustomStackFull(CustomStack *customStack) {
    return (customStack->top == MAX_STACK_SIZE - 1);
}

// Push item onto stack
void pushToCustomStack(CustomStack *customStack, int item) {
    if (!isCustomStackFull(customStack)) {
        customStack->items[++customStack->top] = item;
    }
}

// Pop item from stack
int popFromCustomStack(CustomStack *customStack) {
    if (!isCustomStackEmpty(customStack)) {
        return customStack->items[customStack->top--];
    }
    return -1; // Error: Stack underflow
}


bool coaccessibility(State** des, int numStates, int startStateId) {
    bool visited[numStates];
    for (int i = 0; i < numStates; ++i) {
        visited[i] = false;
    }

    // Depth-first search
    CustomStack customStack;
    initializeCustomStack(&customStack);
    pushToCustomStack(&customStack, startStateId);
    printf("\n");
    while (!isCustomStackEmpty(&customStack)) {
        int currentStateId = popFromCustomStack(&customStack);
        visited[currentStateId] = true;
        printf("%d->",currentStateId);

        // Check if current state is marked
        if (des[currentStateId]->marked) {
            return true; // Found a marked state, return true
        }

        // Visit adjacent states
        for (int i = 0; i < des[currentStateId]->numTransitions; ++i) {
            int nextStateId = des[currentStateId]->transitions[i].targetState;
            if (!visited[nextStateId]) {
                pushToCustomStack(&customStack, nextStateId);
            }
        }
    }

    return false; // No marked state reachable from startStateId
}



// Function to delete a state and its associated transitions
//Here we are passing the position in the array of pointers and not the actual id...
void deleteState(State** des, int stateId, int* numStates) {
    // Free memory associated with transitions
    free(des[stateId]->transitions);
    // Free memory associated with state
    free(des[stateId]);


    // Move states after the deleted one to fill the gap
    for (int i = stateId; i < *numStates-1; ++i) {
        des[i] = des[i+1];    
    }

    // Decrement the total number of states
    *numStates= *numStates - 1;
}

// Function to iterate through the DES and remove non-coaccessible states
void trim(State** des, int* numStates) {
    int id_removed;
    for (int i = 0; i < *numStates; ++i) {
        
        if (!coaccessibility(des, *numStates, i)){
            id_removed=des[i]->id;
            // Remove transitions to the deleted state from other states
            // If state is not coaccessible, delete it
            deleteState(des, i, numStates);//The no of states decreases and the states are shifted....
            for (int j = 0; j < *numStates; ++j) {
                for (int k = 0; k < des[j]->numTransitions; ++k) {
                    if (des[j]->transitions[k].targetState == id_removed) {
                        // Shift transitions after the deleted one to fill the gap
                        for (int l = k + 1; l < des[j]->numTransitions; ++l) {
                            des[j]->transitions[l - 1] = des[j]->transitions[l];
                        }
                        // Decrement the number of transitions
                        des[j]->numTransitions--;
                    }
                }
            }
            i--;
            
        }
        
    }
}