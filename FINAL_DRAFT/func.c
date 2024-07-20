#include "func.h"
//PROCESS OF CREATING A NEW STATE.
State* createState(int id,bool marked,int maxTransitions){
   State* newState = (State*) malloc(sizeof(State));
   if(newState==NULL){
      printf("FAILED TO CREATE STATE\n");
      exit(1);
   }
   newState->id = id;
   newState->marked = marked;
   newState->maxTransitions = maxTransitions;
   newState->numTransitions = 0;
   newState->transitions = (Transition*) malloc(sizeof(Transition)*maxTransitions);
   if(newState->transitions==NULL){
      printf("FAILED TO ALLOCATE MEMORY FOR TRANSITIONS\n");
      exit(1);
   }
   return newState;
}




//PROCESS OF ADDING A NEW TRANSITION TO THE STATE.
void addTransition(State* state,int targetState,char label){
   if (state->numTransitions >= state->maxTransitions) {
          printf("MAXIMUM NUMBER OF TRANSITIONS REACHED FOR STATE %d\n", state->id);
          return;
      }
   Transition* newTransition= &state->transitions[state->numTransitions];
   newTransition->targetState=targetState;
   newTransition->label=label;
   state->numTransitions++;
}





void writeDESToFile(State** des, int numStates, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%d\n", numStates); // Writing the number of states

    // Iterate through each state
    for (int i = 0; i < numStates; i++) {
        State* state = des[i];
        fprintf(file, "%d %d %d\n", state->id, state->marked, state->numTransitions); // Writing state info

        // Iterate through transitions of the state
        for (int j = 0; j < state->numTransitions; j++) {
            Transition transition = state->transitions[j];
            fprintf(file, "%d %c\n", transition.targetState, transition.label); // Writing transition info
        }
    }

    fclose(file);
}






DESResult readDESFromFile(const char* filename) {
    DESResult result;
    int targetState, id, marked, numTransitions;
    char label;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        result.des = NULL;
        result.numStates = -1; // Indicate error
        return result;
    }

    fscanf(file, "%d", &result.numStates); // Read the number of states

    result.des = (State**)malloc(result.numStates * sizeof(State*));
    if (result.des == NULL) {
        printf("Failed to allocate memory for DES\n");
        fclose(file);
        result.numStates = -1; // Indicate error
        return result;
    }

    // Read each state's information
    for (int i = 0; i < result.numStates; i++) {
        fscanf(file, "%d %d %d", &id, &marked, &numTransitions);

        // Create the state
        result.des[i] = createState(id, marked, numTransitions);

        // Read each transition's information
        for (int j = 0; j < numTransitions; j++) {
            fscanf(file, "%d %c", &targetState, &label);
            addTransition(result.des[i], targetState, label);
        }
    }

    fclose(file);
    return result;
}