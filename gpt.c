#include <stdio.h>
#include <stdlib.h>

#define NUM_COLORS 4
#define BEAM_WIDTH 4

typedef struct {
    int *colors;
    int cost;
} Candidate;

// Функція для перевірки, чи є конфлікти кольорів
int isValid(int *colors, int numNodes, int **graph) {
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (graph[i][j] == 1 && colors[i] == colors[j]) {
                return 0;  // є конфлікт
            }
        }
    }
    return 1;  // немає конфліктів
}

// Функція для оцінки поточного розфарбування (в даному випадку використовуємо кількість конфліктів)
int evaluate(int *colors, int numNodes, int **graph) {
    int conflicts = 0;
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (graph[i][j] == 1 && colors[i] == colors[j]) {
                conflicts++;
            }
        }
    }
    return conflicts;
}

// Функція для генерації всіх можливих кольорів для певного вузла
void generateCandidates(int *colors, int numNodes, int node, int **graph, Candidate *candidates) {
    for (int color = 0; color < NUM_COLORS; color++) {
        // Призначаємо поточний колір
        colors[node] = color;
        
        // Перевіряємо, чи не виникає конфліктів
        if (isValid(colors, numNodes, graph)) {
            // Оцінюємо розфарбування і додаємо в кандидатів
            candidates[color].colors[node] = color;
            candidates[color].cost = evaluate(colors, numNodes, graph);
        }
    }
}

// Beam Search для розфарбування графа
int beamSearch(int **graph, int numNodes) {
    int *colors = (int *)malloc(numNodes * sizeof(int));
    Candidate *candidates = (Candidate *)malloc(BEAM_WIDTH * sizeof(Candidate));
    int currentBeamWidth = BEAM_WIDTH;

    // Початкове розфарбування
    for (int i = 0; i < numNodes; i++) {
        colors[i] = -1;  // -1 означає, що вузол не розфарбований
    }

    for (int step = 0; step < numNodes; step++) {
        // Генеруємо кандидатів для поточного вузла
        generateCandidates(colors, numNodes, step, graph, candidates);

        // Вибір найкращих кандидатів з BEAM_WIDTH
        int minCost = 1000000;
        for (int i = 0; i < BEAM_WIDTH; i++) {
            if (candidates[i].cost < minCost) {
                minCost = candidates[i].cost;
                for (int j = 0; j < numNodes; j++) {
                    colors[j] = candidates[i].colors[j];
                }
            }
        }
    }

    // Повертаємо результат розфарбування
    free(candidates);
    free(colors);
    return 0;  // 0 означає успішне завершення
}

int main() {
    int numNodes = 5;
    // Створюємо простий граф у вигляді списку суміжності
    int **graph = (int **)malloc(numNodes * sizeof(int *));
    for (int i = 0; i < numNodes; i++) {
        graph[i] = (int *)malloc(numNodes * sizeof(int));
    }

    // Заповнюємо граф (суміжність між вузлами)
    // Наприклад, для графа з 5 вузлами:
    graph[0][1] = graph[1][0] = 1;
    graph[1][2] = graph[2][1] = 1;
    graph[2][3] = graph[3][2] = 1;
    graph[3][4] = graph[4][3] = 1;
    graph[0][3] = graph[3][0] = 1;

    // Виконуємо Beam Search
    beamSearch(graph, numNodes);

    // Звільнення пам'яті
    for (int i = 0; i < numNodes; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}
