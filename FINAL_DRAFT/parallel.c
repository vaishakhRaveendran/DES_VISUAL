#include "parallel.h"
char* events1,*events2;

char* getAssociatedEvents(DESResult result) {
    bool eventSet[128] = {false}; 

    // Iterate through each state
    for (int i = 0; i < result.numStates; i++) {
        State *state = result.des[i];

        // Iterate through transitions of each state
        for (int j = 0; j < state->numTransitions; j++) {
            Transition transition = state->transitions[j];
            char event = transition.label;

            // Add event to the set if it's not already present
            eventSet[event] = true;
        }
    }

    
    char* associatedEvents = (char*)malloc(128 * sizeof(char));
    int count = 0;

    // Store the collected events in the array
    for (int i = 0; i < 128; i++) {
        if (eventSet[i]) {
            associatedEvents[count++] = i;
        }
    }
    associatedEvents[count] = '\0'; 
    return associatedEvents;
}


int check_marked(State* s1, State* s2) {
    if (s1->marked && s2->marked) {
        return 1;
    } else {
        return 0;
    }
}

eventResult findEvents(State* state1, State* state2) {
    eventResult result;
    result.commonCount = 0;
    result.commonLabels = malloc(128 * sizeof(char));
    result.exclusiveCount1 = 0;
    result.exclusiveLabels1 = malloc(128 * sizeof(char));
    result.exclusiveCount2 = 0;
    result.exclusiveLabels2 = malloc(128 * sizeof(char));

    if (state1 == NULL || state2 == NULL) {
        printf("Invalid states.\n");
        return result; 
    } 

    bool labelSet1[128] = {false};
    bool labelSet2[128] = {false};

    // Find common labels
    //Identify all the events belongs to state1.
    for (int i = 0; i < state1->numTransitions; i++) {
        Transition transition = state1->transitions[i];
        char label = transition.label;
        labelSet1[label] = true;
    }

    //Identify common events ,events only belonging to state2 and remove the common events form state1.
    for (int i = 0; i < state2->numTransitions; i++) {
        Transition transition = state2->transitions[i];
        char label = transition.label;
        if (labelSet1[label]) {
            result.commonLabels[result.commonCount++] = label;
            labelSet1[label] = false;// Mark as found in state1
        } else {
            labelSet2[label] = true; //Store exclusive labels for state2
        }
    }

    for (int i = 0; events2[i] != 0; ++i)
    {
        char label = events2[i];
        if (labelSet1[label]) {
            labelSet1[label] = false; 
        }
    }

    for (int i = 0; events1[i] != 0; ++i)
    {
        char label = events1[i];
        if (labelSet2[label]) {
            labelSet2[label] = false; 
        }
    }

    for (int i = 0; i < state1->numTransitions; i++) {
        Transition transition = state1->transitions[i];
        char label = transition.label;
        if (labelSet1[label]) {
            result.exclusiveLabels1[result.exclusiveCount1++] = label;
        }
    }

    for (int i = 0; i < state2->numTransitions; i++) {
        Transition transition = state2->transitions[i];
        char label = transition.label;
        if (labelSet2[label]) {
            result.exclusiveLabels2[result.exclusiveCount2++] = label; // Store exclusive labels for state2
        }
    }

    // Null-terminate label arrays
    result.commonLabels[result.commonCount] = '\0';
    result.exclusiveLabels1[result.exclusiveCount1] = '\0';
    result.exclusiveLabels2[result.exclusiveCount2] = '\0';

    return result;
}


State** parallelComposition(){
    char filename1[100];
    char filename2[100];
    char filename[100];
    eventResult commonEvents;
    char currentEvent;
    int numStates=0,numNodes=0,maxStates,final1,final2,final,stateId,numTransitions;
    State** des;
    Node *Head,*temp;
    Stack stack;
    tuple newState={0,0},tempTuple;
    DESResult result1, result2;

    printf("Enter filename for the first DES:");
    scanf("%s", filename1);
    result1 = readDESFromFile(filename1);
    events1 =getAssociatedEvents(result1);

    if (result1.des == NULL || result1.numStates == -1) {
        printf("Failed to read the first DES from file.\n");
        return NULL;
    }
    printf("Enter filename for the second DES:");
    scanf("%s", filename2);
    result2 = readDESFromFile(filename2);
    events2 =getAssociatedEvents(result2);

    if (result2.des == NULL || result2.numStates == -1) {
        printf("Failed to read the second DES from file.\n");
        free(result1.des); // Free memory allocated for the first DES
        return NULL;
    }

    // Now you have successfully read both DES from files
    State** des1 = result1.des;
    State** des2 = result2.des;
    int numStates1 = result1.numStates;
    int numStates2 = result2.numStates;

    // Perform product composition with des1 and des2
    maxStates=numStates1*numStates2;
    des=(State**)malloc(maxStates * sizeof(State*));
    initializeStack(&stack);
    //Keep track {0,0} pushed.
    Head=createNode(NULL,0,0,numNodes++);
    temp=Head;

    //PRODUCT.
    push(&stack,newState);
    while(!isEmpty(&stack))
    { 
       newState=pop(&stack);
       commonEvents=findEvents(des1[newState.id1],des2[newState.id2]);
       numTransitions=commonEvents.commonCount+commonEvents.exclusiveCount1+commonEvents.exclusiveCount2;
       stateId=isNodeExists(Head,newState.id1,newState.id2);
       des[stateId] = createState(stateId,check_marked(des1[newState.id1],des2[newState.id2]),numTransitions);
       //Adding transitions for common events
       for (int i = 0; commonEvents.commonLabels[i] !='\0'; i++) {
          currentEvent = commonEvents.commonLabels[i];
          final1=getFinalStateByLabel(des1[newState.id1],currentEvent);
          final2=getFinalStateByLabel(des2[newState.id2],currentEvent);
          if((final=isNodeExists(Head,final1,final2))==-1)
          { 
             temp=createNode(temp,final1,final2,numNodes);
             addTransition(des[stateId],numNodes,currentEvent);
             tempTuple.id1=final1;
             tempTuple.id2=final2;

             push(&stack,tempTuple);
             numNodes++;
          }
          else{
            addTransition(des[stateId],final,currentEvent);
          }
          
        }
         
      //Adding transitions where only there is state change for des1
        for (int i = 0; commonEvents.exclusiveLabels1[i] !='\0'; i++) {
          currentEvent = commonEvents.exclusiveLabels1[i];
          final1=getFinalStateByLabel(des1[newState.id1],currentEvent);
          final2=newState.id2;
          if((final=isNodeExists(Head,final1,final2))==-1)
          { 
             temp=createNode(temp,final1,final2,numNodes);
             addTransition(des[stateId],numNodes,currentEvent);
             tempTuple.id1=final1;
             tempTuple.id2=final2;

             push(&stack,tempTuple);
             numNodes++;
          }
          else{
            addTransition(des[stateId],final,currentEvent);
          }
          
        }
        //Adding transitions where only there is state change for des2
        for (int i = 0; commonEvents.exclusiveLabels2[i] !='\0'; i++) {
          currentEvent = commonEvents.exclusiveLabels2[i];
          final1=newState.id1;
          final2=getFinalStateByLabel(des2[newState.id2],currentEvent);
          if((final=isNodeExists(Head,final1,final2))==-1)
          { 
             temp=createNode(temp,final1,final2,numNodes);
             addTransition(des[stateId],numNodes,currentEvent);
             tempTuple.id1=final1;
             tempTuple.id2=final2;

             push(&stack,tempTuple);
             numNodes++;
          }
          else{
            addTransition(des[stateId],final,currentEvent);
          }
          
        }
       numStates++;
    }
    generateGraph(des,numStates);
    printf("Enter filename to save DES:");
    scanf("%s", filename);
    writeDESToFile(des,numStates,filename);
    // Free memory allocated for DES
    for (int i = 0; i < numStates1; i++) {
        if (des1[i] != NULL) {
            free(des1[i]->transitions);
            free(des1[i]);
        }
    }
    free(des1);

    for (int i = 0; i < numStates2; i++) {
        if (des2[i] != NULL) {
            free(des2[i]->transitions);
            free(des2[i]);
        }
    }
    free(des2);
    
    return NULL; 
}
