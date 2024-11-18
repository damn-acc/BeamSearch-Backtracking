#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NODES 47
#define COLORS 4

#define SHOW_EXECUTION 0

const char *colors[] = {"Red", "Yellow", "Blue", "Green"};

int** createGraph(int size);
void printGraph(int** graph, int size);
int backtracking(int state[], int current_node, int** graph);
void getAvailableColors(int node, int state[], int available[], int** graph);


int main(){
    int graph_size = NODES;
    int** graph = createGraph(graph_size);
    // printGraph(graph, graph_size);

    int state[NODES];
    memset(state, -1, sizeof(state));

    srand(time(NULL));
    int start_node = rand() % NODES;
    printf("Start node: %d\n", start_node);

    if (backtracking(state, start_node, graph)){
        printf("Graph colouring is successful:\n");
        for (int i = 0; i < NODES; i++) {
            printf("Node %d: color %s\n", i + 1, colors[state[i]]);
        }
    } else{
        printf("Error: Unable to color the graph with %d colors.\n", COLORS);
    }

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

int backtracking(int state[], int current_node, int** graph){
    if (SHOW_EXECUTION){
        printf("Current node = %d\n", current_node);
    }
    int all_colored = 1;
    for (int i = 0; i < NODES; i++){
        if (state[i] == -1){
            all_colored = 0;
            break;
        }
    }
    if (all_colored){
        if (SHOW_EXECUTION){
            printf("All nodes are colored. Returning success.\n");
        }
        return 1;
    } 

    int available[COLORS];
    getAvailableColors(current_node, state, available, graph);

    for (int color = 0; color < COLORS; color++){
        if (available[color]){
            state[current_node] = color;
            if (SHOW_EXECUTION){
                printf("    Node %d colored with %s\n", current_node, colors[color]);
            }
            int success = 1;
            for (int i = 0; i < NODES; i++){
                if (graph[current_node][i] && state[i] == -1){
                    if (SHOW_EXECUTION){
                        printf("    Recurse to node %d (neighbor of %d)\n", i, current_node);
                    }
                    if (!backtracking(state, i, graph)){
                        success = 0;
                        break;
                    }
                }
            }
            if (success){
                return 1;
            }
            if (SHOW_EXECUTION){
                printf("    Undo coloring for node %d\n", current_node);
            }
            state[current_node] = -1;
        }
    }
    if (SHOW_EXECUTION){
        printf("    No valid color for node %d, backtracking...\n", current_node);
    }
    return 0;
}

void getAvailableColors(int node, int state[], int available[], int** graph){
    for (int i = 0; i < COLORS; i++){
        available[i] = 1;
    }
    for (int i = 0; i < NODES; i++){
        if (graph[node][i] && state[i] != -1) {
            available[state[i]] = 0;
        }
    }
    if (SHOW_EXECUTION){
        printf("Available colors for node %d: ", node);
        for (int i = 0; i < COLORS; i++){
            if (available[i]){
                printf("%s ", colors[i]);
            }
        }
        printf("\n");
    }
}