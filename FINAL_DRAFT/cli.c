#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"
#include "func.h"
#include "product.h"
#include "parallel.h"
#include "trim.h"
#include "synthesiser.h"

int main() {
    int maxStates = 0;
    int numStates = 0;
    int choice = 0;
    int maxTransitions;
    int marked, coacc_Id;
    int sourceStateId;
    int targetStateId;
    char filename[100];
    DESResult result;

label:
    printf("\nMENU:\n");
    printf("1. CREATE DES\n");
    printf("2. PRINT DES\n");
    printf("3. SAVE DES TO FILE\n");
    printf("4. UPLOAD DES FROM FILE\n");
    printf("5. PRODUCT COMPOSITION\n");
    printf("6. PARALLEL COMPOSITION\n");
    printf("7. TRIM DES..\n");
    printf("8. SAFE STATE SYNTHESISER..\n");
    printf("9. EXIT\n");

    while (1) {
        printf("ENTER YOUR CHOICE: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("ENTER THE MAX NUMBER OF STATES(0 TO N-1): ");
                scanf("%d", &maxStates);

                State **des = (State **)malloc(maxStates * sizeof(State *));
                if (des == NULL) {
                    printf("FAILED TO CREATE DES\n");
                    exit(1);
                }

                numStates = 0; // Reset numStates

                // Add states
                while (numStates < maxStates) {
                    printf("ENTER MAXIMUM NUMBER OF TRANSITIONS FOR THE STATE[%d]: ",numStates);
                    scanf("%d", &maxTransitions);
                    printf("IS IT A MARKED STATE? (1 FOR YES, 0 FOR NO): ");
                    scanf("%d", &marked);
                    des[numStates] = createState(numStates, marked, maxTransitions);

                    // Add transitions
                    printf(" ******** ADD TRANSTIONS FOR STATE[%d]*********\n",numStates);
                    for (int i = 0; i < maxTransitions; i++) {
                        printf("ENTER TARGET STATE ID (0 TO %d) FOR TRANSITION %d: ", maxStates, i + 1);
                        scanf("%d", &targetStateId);
                        printf("ENTER TRANSITION LABEL: ");
                        char label;
                        scanf(" %c", &label);
                        addTransition(des[numStates], targetStateId, label);
                    }

                    printf("STATE WITH ID %d ADDED SUCCESSFULLY\n", numStates);
                    numStates++;
                }
                break;
            case 2:
                printf("****GENERATING GRAPH*****\n");
                generateGraph(des, numStates);
                break;
            case 3:
                printf("Enter filename to save DES:");
                scanf("%s", filename);
                writeDESToFile(des, numStates, filename);
                break;
            case 4:
                printf("Enter filename to upload DES:");
                scanf("%s", filename);
                result = readDESFromFile(filename);
                if (result.des == NULL || result.numStates == -1) {
                    printf("Failed to read DES from file.\n");
                } else {
                    des = result.des;
                    numStates = result.numStates;
                }
                printf("%d\n", numStates);
                break;
            case 5:
                productComposition();
                break;
            case 6:
                parallelComposition();
                break;
            case 7:
                printf("Enter filename to upload DES:");
                scanf("%s", filename);
                result = readDESFromFile(filename);
                if (result.des == NULL || result.numStates == -1) {
                    printf("Failed to read DES from file.\n");
                } else {
                    des = result.des;
                    numStates = result.numStates;
                }
                trim(des, &numStates);
                generateGraph(des, numStates);
                writeDESToFile(des, numStates, "remove.txt");
                break;
            case 8:
                safe_state_synthesiser();
                break;
            case 9:
                // Exit
                printf("EXITING...\n");
                // Free dynamically allocated memory
                for (int i = 0; i < numStates; i++) {
                    if (des[i] != NULL) {
                        free(des[i]->transitions);
                        free(des[i]);
                    }
                }
                free(des);
                printf("FREEING MEMORY\n");
                exit(0);
            default:
                printf("INVALID CHOICE! PLEASE ENTER A VALID OPTION.\n");
        }
    }

    return 0;
}
