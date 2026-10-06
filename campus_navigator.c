#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX 20
#define INF 99999
#define WIDTH 90
#define HEIGHT 32

int graph[MAX][MAX], distance[MAX], visited[MAX], parent[MAX];
char location[MAX][50];
int n = 0, source = -1, calculated = 0;

void displayLocations() {
    int i;
    printf("\nAvailable Locations:\n");
    for (i = 0; i < n; i++) printf("%d. %s\n", i + 1, location[i]);
}

void initializeDefaultGraph() {
    int i, j;
    char defaults[5][50] = {"Main Gate", "Library", "Computer Dept", "Laboratory", "Auditorium"};
    n = 5;
    for (i = 0; i < 5; i++) strcpy(location[i], defaults[i]);
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++) graph[i][j] = (i == j) ? 0 : INF;

    graph[0][1] = graph[1][0] = 4;
    graph[0][2] = graph[2][0] = 10;
    graph[1][2] = graph[2][1] = 3;
    graph[1][3] = graph[3][1] = 5;
    graph[2][3] = graph[3][2] = 2;
    graph[2][4] = graph[4][2] = 8;
    graph[3][4] = graph[4][3] = 6;
    source = -1; calculated = 0;
}

void displayMenu() {
    printf("\n====================================================\n");
    printf("          CAMPUS SHORTEST ROUTE FINDER\n              DIJKSTRA'S ALGORITHM\n");
    printf("====================================================\n");
    printf("1.  Display Campus Graph\n2.  Display Visual Campus Map\n3.  Display Adjacency Matrix\n");
    printf("4.  Select Source Location\n5.  Find Shortest Distance\n6.  Display Shortest Paths\n");
    printf("7.  Display Distance from Source\n8.  Add New Location\n9.  Add New Road\n");
    printf("10. Enter Completely New Campus Graph\n11. Exit\n");
    printf("====================================================\nEnter your choice: ");
}

void displayGraph() {
    int i, j, roads = 0;
    printf("\n====================================================\n                CAMPUS ROAD NETWORK\n====================================================\n");
    if (n == 0) { printf("No locations available.\n"); return; }
    displayLocations();
    printf("\nRoads:\n");
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (graph[i][j] != INF && graph[i][j] != 0) {
                printf("[%s] <---- %d m ----> [%s]\n", location[i], graph[i][j], location[j]);
                roads++;
            }
        }
    }
    if (roads == 0) printf("No roads have been added.\n");
}

void displayVisualMap() {
    char canvas[HEIGHT][WIDTH];
    int posX[MAX], posY[MAX];
    int i, j, k, y, cx, cy, radiusX, radiusY;
    double angle;

    printf("\n====================================================\n              VISUAL CAMPUS MAP\n====================================================\n");
    if (n == 0) { printf("No locations available.\n"); return; }

    for (y = 0; y < HEIGHT; y++) {
        memset(canvas[y], ' ', WIDTH - 1);
        canvas[y][WIDTH - 1] = '\0';
    }

    cx = WIDTH / 2; cy = HEIGHT / 2; radiusX = 30; radiusY = 11;
    for (i = 0; i < n; i++) {
        angle = (2.0 * 3.141592653589793 * i / n) - (3.141592653589793 / 2.0);
        posX[i] = cx + (int)(radiusX * cos(angle));
        posY[i] = cy + (int)(radiusY * sin(angle));
    }

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (graph[i][j] != INF && graph[i][j] != 0) {
                int dx = posX[j] - posX[i], dy = posY[j] - posY[i];
                int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
                int mx, my;
                char weight[20];
                if (steps == 0) steps = 1;

                for (k = 1; k < steps; k++) {
                    int x = posX[i] + (dx * k) / steps, py = posY[i] + (dy * k) / steps;
                    if (x >= 0 && x < WIDTH && py >= 0 && py < HEIGHT) {
                        if (abs(dx) > 0 && abs(dy) > 0)
                            canvas[py][x] = ((dx > 0 && dy > 0) || (dx < 0 && dy < 0)) ? '\\' : '/';
                        else
                            canvas[py][x] = (abs(dx) >= abs(dy)) ? '-' : '|';
                    }
                }
                sprintf(weight, "%d", graph[i][j]);
                mx = (posX[i] + posX[j]) / 2; my = (posY[i] + posY[j]) / 2;
                for (k = 0; weight[k] && mx + k < WIDTH && my >= 0 && my < HEIGHT; k++)
                    canvas[my][mx + k] = weight[k];
            }
        }
    }

    for (i = 0; i < n; i++) {
        char node[10];
        sprintf(node, "[%d]", i + 1);
        for (k = 0; node[k]; k++) {
            int x = posX[i] - 1 + k, py = posY[i];
            if (x >= 0 && x < WIDTH && py >= 0 && py < HEIGHT) canvas[py][x] = node[k];
        }
    }

    printf("\n");
    for (y = 0; y < HEIGHT; y++) printf("%s\n", canvas[y]);
    displayLocations();
    printf("\nNumbers on roads represent distance in meters.\n");
}

void displayMatrix() {
    int i, j;
    printf("\n====================================================\n                ADJACENCY MATRIX\n====================================================\n");
    if (n == 0) { printf("No locations available.\n"); return; }
    printf("\n%-6s", "");
    for (i = 0; i < n; i++) printf("%-12d", i + 1);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%-6d", i + 1);
        for (j = 0; j < n; j++) {
            if (graph[i][j] == INF) printf("%-12s", "INF");
            else printf("%-12d", graph[i][j]);
        }
        printf("\n");
    }
    printf("\nLocation Numbering:\n");
    for (i = 0; i < n; i++) printf("%d = %s\n", i + 1, location[i]);
}

void selectSource() {
    int choice;
    if (n == 0) { printf("\nNo locations available.\n"); return; }
    displayLocations();
    printf("\nEnter source location number: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > n) { printf("\nInvalid location number.\n"); return; }
    source = choice - 1;
    calculated = 0;
    printf("\nSource location selected: %s\nUse Option 5 to run Dijkstra's algorithm.\n", location[source]);
}

int findMinDistance() {
    int i, min = INF, minIndex = -1;
    for (i = 0; i < n; i++) {
        if (!visited[i] && distance[i] < min) {
            min = distance[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void dijkstra() {
    int i, j, u;
    if (source == -1) { printf("\nPlease select a source location first using Option 4.\n"); return; }
    for (i = 0; i < n; i++) {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    distance[source] = 0;

    for (i = 0; i < n - 1; i++) {
        u = findMinDistance();
        if (u == -1) break;
        visited[u] = 1;
        for (j = 0; j < n; j++) {
            if (!visited[j] && graph[u][j] != INF && graph[u][j] != 0 &&
                distance[u] != INF && distance[u] + graph[u][j] < distance[j]) {
                distance[j] = distance[u] + graph[u][j];
                parent[j] = u;
            }
        }
    }
    calculated = 1;
    printf("\n====================================================\n          DIJKSTRA'S ALGORITHM COMPLETED\n====================================================\n");
    printf("Source: %s\n\nShortest distances calculated successfully.\nUse Option 6 to display shortest paths.\nUse Option 7 to display distances.\n", location[source]);
}

void printPath(int vertex) {
    if (parent[vertex] == -1) { printf("%s", location[vertex]); return; }
    printPath(parent[vertex]);
    printf(" -> %s", location[vertex]);
}

void displayShortestPaths() {
    int i;
    if (source == -1) { printf("\nPlease select a source location first.\n"); return; }
    if (!calculated) { printf("\nPlease run Option 5 first.\n"); return; }
    printf("\n====================================================\n                 SHORTEST PATHS\n====================================================\n");
    printf("Source: %s\n\n", location[source]);
    for (i = 0; i < n; i++) {
        printf("%s: ", location[i]);
        if (distance[i] == INF) printf("No path available\n");
        else { printPath(i); printf("  [Distance = %d m]\n", distance[i]); }
    }
}

void displayDistances() {
    int i;
    if (source == -1) { printf("\nPlease select a source location first.\n"); return; }
    if (!calculated) { printf("\nPlease run Option 5 first.\n"); return; }
    printf("\n====================================================\n         SHORTEST DISTANCE FROM SOURCE\n====================================================\nSource: %s\n\n", location[source]);
    printf("%-5s %-25s %-15s\n----------------------------------------------------\n", "No.", "Location", "Distance");
    for (i = 0; i < n; i++) {
        printf("%-5d %-25s ", i + 1, location[i]);
        if (distance[i] == INF) printf("%-15s\n", "Unreachable");
        else printf("%-12d m\n", distance[i]);
    }
}

void addLocation() {
    int i;
    if (n >= MAX) { printf("\nMaximum number of locations reached.\n"); return; }
    printf("\n====================================================\n                 ADD NEW LOCATION\n====================================================\nEnter name of new location: ");
    scanf(" %[^\n]", location[n]);
    for (i = 0; i < n; i++) graph[n][i] = graph[i][n] = INF;
    graph[n][n] = 0;
    n++;
    calculated = 0;
    printf("\nNew location added successfully!\nLocation Number: %d\nLocation Name: %s\n\nUse Option 9 to connect this location to another location.\nThe updated graph is now available in Option 2.\n", n, location[n - 1]);
}

void addRoad() {
    int from, to, distanceValue;
    if (n < 2) { printf("\nAt least two locations are required.\n"); return; }
    printf("\n====================================================\n                   ADD NEW ROAD\n====================================================\n");
    displayLocations();
    printf("\nEnter starting location number: ");
    scanf("%d", &from);
    printf("Enter destination location number: ");
    scanf("%d", &to);
    if (from < 1 || from > n || to < 1 || to > n) { printf("\nInvalid location number.\n"); return; }
    if (from == to) { printf("\nA location cannot be connected to itself.\n"); return; }
    printf("Enter road distance in meters: ");
    scanf("%d", &distanceValue);
    if (distanceValue <= 0) { printf("\nDistance must be greater than zero.\n"); return; }
    graph[from - 1][to - 1] = graph[to - 1][from - 1] = distanceValue;
    calculated = 0;
    printf("\nRoad added successfully!\n[%s] <---- %d m ----> [%s]\nThe updated road will appear in the same\nASCII graph when Option 2 is selected.\n", location[from - 1], distanceValue, location[to - 1]);
}

void enterNewGraph() {
    int i, j, roads, from, to, distanceValue;
    printf("\n====================================================\n           ENTER NEW CAMPUS GRAPH\n====================================================\nEnter number of locations (maximum %d): ", MAX);
    scanf("%d", &n);
    if (n <= 0 || n > MAX) { printf("\nInvalid number of locations.\n"); n = 0; return; }
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++) graph[i][j] = (i == j) ? 0 : INF;
    for (i = 0; i < n; i++) {
        printf("Enter name of location %d: ", i + 1);
        scanf(" %[^\n]", location[i]);
    }
    printf("\nEnter number of roads: ");
    scanf("%d", &roads);
    if (roads < 0) roads = 0;
    for (i = 0; i < roads; i++) {
        printf("\nRoad %d\n", i + 1);
        displayLocations();
        printf("Enter starting location number: ");
        scanf("%d", &from);
        printf("Enter destination location number: ");
        scanf("%d", &to);
        printf("Enter distance in meters: ");
        scanf("%d", &distanceValue);
        if (from >= 1 && from <= n && to >= 1 && to <= n && from != to && distanceValue > 0)
            graph[from - 1][to - 1] = graph[to - 1][from - 1] = distanceValue;
        else
            printf("Invalid road details. Road not added.\n");
    }
    source = -1; calculated = 0;
    printf("\nNew campus graph created successfully!\n");
}

int main() {
    int choice;
    initializeDefaultGraph();
    printf("\n====================================================\n        WELCOME TO CAMPUS SHORTEST ROUTE FINDER\n====================================================\n");
    do {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:  displayGraph(); break;
            case 2:  displayVisualMap(); break;
            case 3:  displayMatrix(); break;
            case 4:  selectSource(); break;
            case 5:  dijkstra(); break;
            case 6:  displayShortestPaths(); break;
            case 7:  displayDistances(); break;
            case 8:  addLocation(); break;
            case 9:  addRoad(); break;
            case 10: enterNewGraph(); break;
            case 11: printf("\n====================================================\n       Thank you for using Campus Route Finder!\n====================================================\n"); break;
            default: printf("\nInvalid choice!\nPlease enter a number from 1 to 11.\n");
        }
    } while (choice != 11);
    return 0;
}
