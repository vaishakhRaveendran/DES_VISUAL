#include "synthesiser.h"
// Initialize stack
void initializeSynStack(SynStack *stack) {
    stack->top = -1;
}

// Check if stack is empty
int isSynStackEmpty(SynStack *stack) {
    return (stack->top == -1);
}

// Check if stack is full
int isSynStackFull(SynStack *stack) {
    return (stack->top == MAX_SYN_STACK_SIZE - 1);
}

// Push item onto stack
void pushToSynStack(SynStack *stack, int item) {
    if (!isSynStackFull(stack)) {
        stack->items[++stack->top] = item;
    }
}

// Pop item from stack
int popFromSynStack(SynStack *stack) {
    if (!isSynStackEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return -1; 
}

// Check if a number is present in the array
bool isPresent(int arr[], int index, int num) {
    for (int i = 0; i < index; i++) {
        if (arr[i] == num) {
            return true; 
        }
    }
    return false; 
}

// Find the union of two arrays
int* findUnion(int *arr1, int size1, int *arr2, int size2, int *unionSize) {
    int maxSize = size1 + size2; // Maximum possible size of the union
    int *unionArr = (int*)malloc(maxSize * sizeof(int));
    if (unionArr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;

    // Add elements from the first array to the union array
    for (int i = 0; i < size1; i++) {
        if (!isPresent(unionArr, index, arr1[i])) {
            unionArr[index++] = arr1[i];
        }
    }

    // Add elements from the second array to the union array
    for (int i = 0; i < size2; i++) {
        if (!isPresent(unionArr, index, arr2[i])) {
            unionArr[index++] = arr2[i];
        }
    }

    *unionSize = index; // Set the size of the union array
    return unionArr;
}


bool reachForbidden(State **des, int numStates, int startStateId, int not_Q_prime[], int len_notQPrime) {
    bool visited[numStates];
    for (int i = 0; i < numStates; ++i) {
        visited[i] = false;
    }

    SynStack SynStack;
    initializeSynStack(&SynStack);
    pushToSynStack(&SynStack, startStateId);

    while (!isSynStackEmpty(&SynStack)) {
        int currentStateId = popFromSynStack(&SynStack);
        visited[currentStateId] = true;

        // Check if the current state is in not_Q_prime
        for (int i = 0; i < len_notQPrime; ++i) {
            if (currentStateId == not_Q_prime[i]) {
                return true;
            }
        }

        // Visit adjacent states
        for (int i = 0; i < des[currentStateId]->numTransitions; ++i) {
            // Check if the target state is uncontrollable
            if (isupper((unsigned char)des[currentStateId]->transitions[i].label)) {
                int nextStateId = des[currentStateId]->transitions[i].targetState;
                nextStateId = findStateIndex(des, numStates, nextStateId);
                if (!visited[nextStateId]) {
                    pushToSynStack(&SynStack, nextStateId);
                }
            }
        }
    }

    return false;
}

int findStateIndex(State** des, int numStates, int stateId) {
    for (int i = 0; i < numStates; i++) {
        if (des[i]->id == stateId) {
            return i; // Return the index of the state if found
        }
    }
    return -1; 
}


// Check coaccessibility
bool syn_coaccessibility(State** des, int numStates, int startStateId,int forbiddenStates[], int* len_forbiddenStates ) {
    bool visited[numStates];
    for (int i = 0; i < numStates; ++i) {
        visited[i] = false;
    }

    // Depth-first search
    SynStack SynStack;
    initializeSynStack(&SynStack);
    pushToSynStack(&SynStack, startStateId);

    while (!isSynStackEmpty(&SynStack)) {
        int currentStateId = popFromSynStack(&SynStack);
        visited[currentStateId] = true;
        
        //Getting index of the state id....
        currentStateId=findStateIndex(des,numStates, currentStateId);

        // Check if current state is marked
        if (des[currentStateId]->marked) {
             
            return true; // Found a marked state, return true
        }

        // Visit adjacent states 
        for (int i = 0; i < des[currentStateId]->numTransitions; ++i) {
            int nextStateId = des[currentStateId]->transitions[i].targetState;
            bool forbiddenFound = false; 

            // Check if nextStateId is forbidden
            for (int j = 0; j < *len_forbiddenStates; ++j) {
                if (forbiddenStates[j] == nextStateId) {
                    forbiddenFound = true;
                    break; // No need to check further
                }
            }

            // If nextStateId is not forbidden and not visited, push it to the stack
            if (!forbiddenFound && !visited[nextStateId]) {
                pushToSynStack(&SynStack, nextStateId);
            } 
        }
           
    }

    return false;//No marked state reachable from startStateId
}


// Perform restricted backwards
int* restrictedBackwards(State** des, int numStates, int forbiddenStates[], int* len_Q_prime, int* len_forbiddenStates) {
    int* Q_prime = (int*)malloc(numStates * sizeof(int));
    int count = 0;

    for (int i = 0; i < numStates; ++i) {
        // Check if the current state can reach a marked state
        if (syn_coaccessibility(des, numStates, des[i]->id,forbiddenStates,len_forbiddenStates)) {
            bool isForbidden = false;
            // Check if the state is in the forbidden list
            for (int j = 0; j < *len_forbiddenStates; ++j) {
                if (forbiddenStates[j] == i) {//forbidden state stores index...
                    isForbidden = true;
                    break;
                }
            }
            // If not forbidden, add it to Q'
            if (!isForbidden) {
                Q_prime[count++] = i;
            }
        }
    }

    *len_Q_prime = count;
    return Q_prime;
}

// Get states not in Q'
int* getStatesNotInQPrime(int numStates, int* Q_prime, int len_Q_prime, int* len_notQPrime) {
    int* notQPrime = (int*)malloc(numStates * sizeof(int));
    int count = 0;

    for (int i = 0; i < numStates; ++i) {
        bool isInQPrime = false;
        for (int j = 0; j < len_Q_prime; ++j) {
            if (i == Q_prime[j]) {
                isInQPrime = true;
                break;
            }
        }
        if (!isInQPrime) {
            notQPrime[count++] = i;
        }
    }

    *len_notQPrime = count;
    return notQPrime;
}

// Perform uncontrolled backwards
int* uncontrolledBackwards(State** des, int numStates,int *len_Q_double_prime,int notQPrime[],int *len_notQPrime) {
    int* Q_double_prime = (int*)malloc(numStates * sizeof(int)); 
    int count = 0; 

    for (int i = 0; i < numStates; ++i) {
        if (reachForbidden(des, numStates,i,notQPrime,*len_notQPrime)) {    
                Q_double_prime[count++] = i;
        }
    }
    *len_Q_double_prime=count;
    return Q_double_prime;
}

// Delete a state
void syn_removeState(State** des, int stateId, int* numStates) {
    free(des[stateId]->transitions);
    free(des[stateId]);

    // Move states after the deleted one to fill the gap
    for (int i = stateId; i < *numStates-1; ++i) {
        des[i] = des[i+1];    
    }

    // Decrement the total number of states
    *numStates= *numStates - 1;
}

// Perform restricted forward
State** restrictedForward(State** des, int* numStates, int startStateId, int forbiddenStates[], int* len_forbiddenStates) {
    // Remove forbidden states
    for (int i = 0; i < *len_forbiddenStates; ++i) {
        int removed_id= des[forbiddenStates[i]]->id;
        syn_removeState(des, forbiddenStates[i],numStates);

        update_forbidden_list(forbiddenStates,len_forbiddenStates,i);

        for (int j = 0; j < *numStates; ++j){
                for (int k = 0; k < des[j]->numTransitions; ++k) {
                    if (des[j]->transitions[k].targetState == removed_id) {
                        for (int l = k + 1; l < des[j]->numTransitions; ++l) {
                            des[j]->transitions[l - 1] = des[j]->transitions[l];
                        }
                        des[j]->numTransitions--;
                    }
                }
            }
            i--;

    }

    bool visited[*numStates];
    for (int i = 0; i < *numStates; ++i) {
        visited[i] = false;
    }

    // Depth-first search
    SynStack SynStack;
    initializeSynStack(&SynStack);
    pushToSynStack(&SynStack, startStateId);

    while (!isSynStackEmpty(&SynStack)) {
        int currentStateId = popFromSynStack(&SynStack);
        visited[currentStateId] = true;
        

        // Visit adjacent states
        for (int i = 0; i < des[currentStateId]->numTransitions; ++i) {
            int nextStateId = des[currentStateId]->transitions[i].targetState;
            nextStateId=findStateIndex(des,*numStates,nextStateId);
            if (!visited[nextStateId]) {
                pushToSynStack(&SynStack, nextStateId);
            }
           
        }
    }
    // Remove states not visited
    for (int i = 0; i < *numStates; ++i) {
        if (!visited[i]){
            int id_removed=findStateIndex(des,*numStates,i);
            syn_removeState(des, i, numStates);//The no of states decreases and the states are shifted....
            for (int j = 0; j < *numStates; ++j) {
                for (int k = 0; k < des[j]->numTransitions; ++k) {
                    if (des[j]->transitions[k].targetState == id_removed) {
                        for (int l = k + 1; l < des[j]->numTransitions; ++l) {
                            des[j]->transitions[l - 1] = des[j]->transitions[l];
                        }
                        des[j]->numTransitions--;
                    }
                }
            }
            i--;
            
        }
        
    }

    // Reallocate memory for the reduced des
    des = realloc(des, (*numStates) * sizeof(State*));

    return des;
}

void update_forbidden_list(int forbiddenStates[], int* len_forbidden, int index) {
    // Shift elements to remove the element at the given index
    int removed_index=forbiddenStates[index];
    for (int i = index; i < *len_forbidden - 1; ++i) {
        forbiddenStates[i] = forbiddenStates[i + 1];
    }
    // Decrease the count of forbidden states
    (*len_forbidden)--;

    // Decrease 1 from the content of the cell if it is greater than the removed cell
    for (int i = 0; i < *len_forbidden; ++i) {
        if (forbiddenStates[i] > removed_index) {
            forbiddenStates[i]--;
        }
    }
}



int safe_state_synthesiser() {
    int i = 0;
    char filename[100];
    int len_Q_prime, len_forbiddenStates, len_forbiddenStates_0, len_Q_double_prime, len_notQPrime;
    int *Q_prime, *not_Q_prime, *Q_double_prime, *forbiddenStates;
    len_Q_prime = 0;
    len_forbiddenStates = 0;
    len_Q_double_prime = 0;
    printf("Enter filename to save DES:");
    scanf("%s", filename);
    DESResult result;
    State **des;
    result = readDESFromFile(filename);

    if (result.des == NULL || result.numStates == -1) {
        printf("Failed to read the first DES from file.\n");
        return -1;
    }
    des = result.des;
    int numStates = result.numStates;
    do {
        len_forbiddenStates_0 = len_forbiddenStates;
        Q_prime = restrictedBackwards(des, numStates, forbiddenStates, &len_Q_prime, &len_forbiddenStates);   
        not_Q_prime = getStatesNotInQPrime(numStates, Q_prime, len_Q_prime, &len_notQPrime);    
        Q_double_prime = uncontrolledBackwards(des, numStates,&len_Q_double_prime, not_Q_prime, &len_notQPrime);    
        forbiddenStates = findUnion(forbiddenStates, len_forbiddenStates, Q_double_prime, len_Q_double_prime, &len_forbiddenStates);    
    } while (len_forbiddenStates_0 != len_forbiddenStates);

    des = restrictedForward(des, &numStates, 0, forbiddenStates, &len_forbiddenStates);
    generateGraph(des, numStates);
    return -1;
}

