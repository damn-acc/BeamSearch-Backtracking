#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NODES 47
#define SHOW_EXECUTION 0

const char *colors[] = {"R", "Y", "B", "G"};

int** createGraph(int size);
void printGraph(int** graph, int size);
int calculateConflicts(int** graph, int colors[]);
void generateRandomState(int state[], int numNodes, int numColors);
void generateSuccessors(int** graph, int currentState[], int numColors, int branchingFactor, int successors[][NODES]);
void selectBestStates(int** graph, int successors[][NODES], int successorCount, int beamWidth, int selected[][NODES]);
void printState(int state[], int numNodes);
void BeamSearch(int** graph, int numColors, int beamWidth, int branchingFactor);

int main() {
    srand(time(NULL));

    int graph_size = NODES;
    int** graph = createGraph(graph_size);
    // printGraph(graph, graph_size);

    int numColors = 4;
    int beamWidth = 3;
    int branchingFactor = 5;

    BeamSearch(graph, numColors, beamWidth, branchingFactor);

    return 0;
}

int** createGraph(int size){
    FILE* file = fopen("matrix.txt", "r");

    int** graph = (int**) malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++){
        graph[i] = (int*) malloc(size * sizeof(int));
    }

    int rows = 0, cols = 0;
    char *line = (char*) malloc(1024 * sizeof(char));
    while (fgets(line, 1024, file) != NULL && rows < size){
        cols = 0;
        char* value = strtok(line, ",");
        while (value != NULL && cols < size){
            graph[rows][cols++] = atoi(value);
            value = strtok(NULL, ",");
        }
        rows++;
    }

    free(line); 
    fclose(file);
    return graph;
}

void printGraph(int** graph, int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
}

int calculateConflicts(int** graph, int colors[]){
    int conflicts = 0;
    for (int i = 0; i < NODES; i++){
        for (int j = i + 1; j < NODES; j++){
            if (graph[i][j] && colors[i] == colors[j]){
                conflicts++;
            }
        }
    }
    return conflicts;
}

void generateRandomState(int state[], int numNodes, int numColors){
    for (int i = 0; i < numNodes; i++){
        state[i] = rand() % numColors;
    }
}

void generateSuccessors(int** graph, int currentState[], int numColors, int branchingFactor, int successors[][NODES]){
    for (int i = 0; i < branchingFactor; i++){
        for (int j = 0; j < NODES; j++){
            successors[i][j] = currentState[j];
        }
        int nodeToChange = rand() % NODES;
        int newColor;
        do {
            newColor = rand() % numColors;
        } while (newColor == currentState[nodeToChange]);
        successors[i][nodeToChange] = newColor;
    }
}

void selectBestStates(int** graph, int successors[][NODES], int successorCount, int beamWidth, int selected[][NODES]){
    int scores[successorCount];
    for (int i = 0; i < successorCount; i++){
        scores[i] = calculateConflicts(graph, successors[i]);
    }

    for (int i = 0; i < beamWidth; i++){
        int bestIndex = -1;
        int bestScore = INT_MAX;
        for (int j = 0; j < successorCount; j++){
            if (scores[j] < bestScore){
                bestScore = scores[j];
                bestIndex = j;
            }
        }
        if (bestIndex != -1){
            for (int k = 0; k < NODES; k++){
                selected[i][k] = successors[bestIndex][k];
            }
            scores[bestIndex] = INT_MAX;
        }
    }
}

void printState(int state[], int numNodes){
    for (int i = 0; i < numNodes; i++){
        printf("%s ", colors[state[i]]);
    }
    printf("\n");
}

void BeamSearch(int** graph, int numColors, int beamWidth, int branchingFactor){
    int initialState[NODES];
    int iteration = 0;
    generateRandomState(initialState, NODES, numColors);
    if (SHOW_EXECUTION){
        printf("Current State 1: ");
        printState(initialState, NODES);
        printf("Conflicts: %d\n", calculateConflicts(graph, initialState));
    }
    if (calculateConflicts(graph, initialState) == 0){
        printf("\nSolution found after %d iterations:\n", iteration);
        printState(initialState, NODES);
        return;
    }
    iteration++;
    int currentStates[beamWidth][NODES];
    int nextStates[beamWidth * branchingFactor][NODES];
    int successorCount = 0;
    int localSuccessors[branchingFactor][NODES];
    generateSuccessors(graph, initialState, numColors, branchingFactor, localSuccessors);
    if (SHOW_EXECUTION){
        printf("\nSuccessors for State 1:\n");
        for (int j = 0; j < branchingFactor; j++){
            printf("Successor %d: ", j + 1);
            printState(localSuccessors[j], NODES);
            printf("Conflicts: %d\n", calculateConflicts(graph, localSuccessors[j]));
        }
    } else{
        for (int j = 0; j < branchingFactor; j++){
            calculateConflicts(graph, localSuccessors[j]);
        }
    }
    for (int j = 0; j < branchingFactor; j++){
        for (int k = 0; k < NODES; k++){
            nextStates[successorCount][k] = localSuccessors[j][k];
        }
        successorCount++;
    }
    if (SHOW_EXECUTION){
        printf("\nSelecting %d best states from %d successors:\n", beamWidth, successorCount);
    }
    selectBestStates(graph, nextStates, successorCount, beamWidth, currentStates);

    while (1) {
        iteration++;
        if (SHOW_EXECUTION){
            printf("\nIteration %d:\n", iteration);
        }
        for (int i = 0; i < beamWidth; i++){
            if (SHOW_EXECUTION){
                printf("Current State %d: ", i + 1);
                printState(currentStates[i], NODES);
                printf("Conflicts: %d\n", calculateConflicts(graph, currentStates[i]));
            } else{
                calculateConflicts(graph, currentStates[i]);
            }
        }

        for (int i = 0; i < beamWidth; i++){
            if (calculateConflicts(graph, currentStates[i]) == 0){
                printf("\nSolution found after %d iterations:\n", iteration);
                printState(currentStates[i], NODES);
                return;
            }
        }
        successorCount = 0;
        for (int i = 0; i < beamWidth; i++){
            int localSuccessors[branchingFactor][NODES];
            generateSuccessors(graph, currentStates[i], numColors, branchingFactor, localSuccessors);

            if (SHOW_EXECUTION){
                printf("\nSuccessors for State %d:\n", i + 1);
            }
            for (int j = 0; j < branchingFactor; j++){
                if (SHOW_EXECUTION){
                    printf("Successor %d: ", j + 1);
                    printState(localSuccessors[j], NODES);
                    printf("Conflicts: %d\n", calculateConflicts(graph, localSuccessors[j]));
                } else{
                    calculateConflicts(graph, localSuccessors[j]);
                }
            }

            for (int j = 0; j < branchingFactor; j++){
                for (int k = 0; k < NODES; k++){
                    nextStates[successorCount][k] = localSuccessors[j][k];
                }
                successorCount++;
            }
        }
        if (SHOW_EXECUTION){
            printf("\nSelecting %d best states from %d successors:\n", beamWidth, successorCount);
        }
        selectBestStates(graph, nextStates, successorCount, beamWidth, currentStates);

        for (int i = 0; i < beamWidth; i++){
            if (SHOW_EXECUTION){
                printf("Selected State %d: ", i + 1);
                printState(currentStates[i], NODES);
                printf("Conflicts: %d\n", calculateConflicts(graph, currentStates[i]));
            } else{
                calculateConflicts(graph, currentStates[i]);
            } 
        }
    }
}
