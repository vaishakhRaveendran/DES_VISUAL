#include "product.h"

// Function to create a new stack node
StackNode* createStackNode(tuple data) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to initialize stack
void initializeStack(Stack* stack) {
    stack->top = NULL;
}

// Function to push a tuple onto the stack
void push(Stack* stack, tuple data) {
    StackNode* newNode = createStackNode(data);
    newNode->next = stack->top;
    stack->top = newNode;
}


// Function to pop a tuple from the stack
tuple pop(Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty.\n");
        exit(EXIT_FAILURE);
    }
    StackNode* temp = stack->top;
    tuple poppedData = temp->data;
    stack->top = stack->top->next;
    free(temp);
    return poppedData;
}

// Function to check if the stack is empty
int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

// Function to free memory allocated to the stack
void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}

//LINKED LIST FOR KEEPING TRACK OF STATES CREATED
// Function to create a new node
Node* createNode(Node* lastNode, int id1, int id2, int id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->id1 = id1;
    newNode->id2 = id2;
    newNode->id = id;
    newNode->next = NULL;

    if (lastNode != NULL) {
        lastNode->next = newNode;
    }

    return newNode;
}


int isNodeExists(Node* startPtr, int id1, int id2) {
    Node* current = startPtr; // Start from the beginning of the linked list

    // Traverse through the linked list
    while (current != NULL) {
        // Check if the current node has the same id1 and id2
        if (current->id1 == id1 && current->id2 == id2) {
            return current->id; // Node with id1 and id2 exists
        }
        current = current->next; // Move to the next node
    }
    
    return -1; // Node with id1 and id2 not found
}



//FUNCTION FOR GETTING FINAL STATE OF A LABEL IN A STATE OF DES
int getFinalStateByLabel(State* state, char label) {
    Transition* transitions = state->transitions;
    int numTransitions = state->numTransitions;

    // Iterate through transitions
    for (int i = 0; i < numTransitions; i++) {
        // Check if the transition's label matches the given label
        if (transitions[i].label == label) {
            // Return the state ID of the destination state
            return transitions[i].targetState;
        }
    }

    // If no matching transition is found, return -1
    return -1;
}


// Function to find common labels between two states
CommonLabelsResult findCommonLabels(State* state1, State* state2) {
    CommonLabelsResult result;
    char label;
    result.count = 0;
    result.labels = malloc(128 * sizeof(char));
    if (state1 == NULL || state2 == NULL) {
        printf("Invalid states.\n");
        return result; 
    }

    bool labelSet[128] = {false}; 

    // Iterate through transitions of state1
    for (int i = 0; i < state1->numTransitions; i++) {
        Transition transition = state1->transitions[i];
         label = transition.label;
         printf("des1:%c\n",label);
        labelSet[label] = true;
    }

    // Iterate through transitions of state2 and check for common labels
    for (int i = 0; i < state2->numTransitions; i++) {
        Transition transition = state2->transitions[i];
        label = transition.label;
        if (labelSet[label]) {
            result.labels[result.count++] = label; 
            printf("des1 and des2 common:%c\n",label);
            labelSet[label] = false; 
        }
    }
    result.labels[result.count++]='\0';
    return result;
}



State** productComposition() {
    char filename1[100];
    char filename2[100];
    char filename[100];
    char currentEvent;
    DESResult result1, result2;
    int numStates=0,numNodes=0,maxStates,final1,final2,final,stateId;
    int* genArray;
    State** des;
    Node *Head,*temp;
    Stack stack;
    tuple newState={0,0},tempTuple;
    CommonLabelsResult commonEvents;

    printf("Enter filename for the first DES:");
    scanf("%s", filename1);
    result1 = readDESFromFile(filename1);

    if (result1.des == NULL || result1.numStates == -1) {
        printf("Failed to read the first DES from file.\n");
        return NULL;
    }



    printf("Enter filename for the second DES:");
    scanf("%s", filename2);
    result2 = readDESFromFile(filename2);

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
       commonEvents=findCommonLabels(des1[newState.id1],des2[newState.id2]);
       stateId=isNodeExists(Head,newState.id1,newState.id2);
       des[stateId] = createState(stateId,0,commonEvents.count);
       for (int i = 0; commonEvents.labels[i] !='\0'; i++) {
          currentEvent = commonEvents.labels[i];
          final1=getFinalStateByLabel(des1[newState.id1],currentEvent);
          final2=getFinalStateByLabel(des2[newState.id2],currentEvent);
          if((final=isNodeExists(Head,final1,final2))==-1)
          { 
             temp=createNode(temp,final1,final2,numNodes);
             //Adding transitions to final states that are not yet created.
             addTransition(des[stateId],numNodes,currentEvent);
             tempTuple.id1=final1;
             tempTuple.id2=final2;
             //Everytime a state is pushed to stack then it was not previously created
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
