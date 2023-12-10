#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define MAX_X_POS 100
#define MAX_Y_POS 35

const char BACKGROUND_CHAR = ' ';
const char CELL_CHAR = '#';
const float SLEEP_TIME = 0.40;

char mainMatrix[MAX_Y_POS][MAX_X_POS];
char neighborCountBuffer[MAX_Y_POS][MAX_X_POS]; // Keep track of the amount of neighbors that each cell has separately

struct Cell{
    int x;
    int y;
};

struct Cell *parseInitialCoordinates(char *s, int *);
void initMatrix();
void setInitialCells(struct Cell c[], size_t);
void conwayLoop();
void updateCells();
void printCells();
int countNeighbors(int, int);

int main(){
    int initialCellsCount = 0;
    bool exit = false;

    char s[10000];
    printf("Input a comma-separated list of x and y coordinates to set the initial cells:\n");
    scanf("%10000[^\n]", s);

    struct Cell *initialCells = parseInitialCoordinates(s, &initialCellsCount);

    initMatrix();
    setInitialCells(initialCells, initialCellsCount);
    conwayLoop();

    return 0;
}

struct Cell *parseInitialCoordinates(char *s, int *initialCellsCount){
    char **coordinates = (char **)calloc(MAX_X_POS * MAX_Y_POS, sizeof(char *));

    if (coordinates == NULL){
        perror("Memory allocation failed. Exiting now");
        exit(EXIT_FAILURE);
    }

    char *input = strdup(s);

    if (input == NULL){
        perror("Memory allocation failed. Exiting now");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char *coordinate = strtok(input, ",");

    while (coordinate != NULL && count < MAX_X_POS * MAX_Y_POS){
        coordinates[count] = strdup(coordinate);

        if (coordinates[count] == NULL){
            perror("Memory allocation failed. Exiting now");
            exit(EXIT_FAILURE);
        }

        count++;

        // Pass null to make it continue tokenizing from where it left off in the same string
        coordinate = strtok(NULL, ",");
    }

    coordinates[count] = NULL;
    free(input);

    struct Cell *cellList = calloc(MAX_X_POS * MAX_Y_POS, sizeof(struct Cell));

    if (cellList == NULL){
        perror("Memory allocation failed. Exiting now");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (coordinates[i] != NULL){
        int x, y;
        // Use sscanf to parse integer values directly
        if (sscanf(coordinates[i], "%d %d", &x, &y) == 2){
            struct Cell c;
            c.x = x;
            c.y = y;
            cellList[i] = c;
        }
        i++;
    }

    *initialCellsCount = i;
    free(coordinates);

    return cellList;
}
void conwayLoop(){
    while (true){
        system("cls");
        printCells();
        updateCells();
        usleep(SLEEP_TIME);
    }
}

void initMatrix(){
    for (int i = 0; i < MAX_Y_POS; i++){
        for (int j = 0; j < MAX_X_POS; j++){
            mainMatrix[i][j] = BACKGROUND_CHAR;
        }
    }
}

/*
    Place initial alive cells in the matrix
*/
void setInitialCells(struct Cell cellList[], size_t size){

    for (int i = 0; i < size; i++){
        struct Cell c = cellList[i];
        mainMatrix[c.y][c.x] = CELL_CHAR;
    }
}

void printCells(){
    for (int i = MAX_Y_POS - 1; i >= 0; i--){
        for (int j = 0; j < MAX_X_POS; j++){
            printf_s("%c", mainMatrix[i][j]);
            neighborCountBuffer[i][j] = countNeighbors(i, j);
        }
        printf_s("\n");
    }
}

/*
    Count surrounding cells for alive neighbors
*/
int countNeighbors(int y, int x){

    int checkCoords[8][2] = {
        {y + 1, x - 1}, // Up left
        {y + 1, x},     // Up
        {y + 1, x + 1}, // Up right
        {y, x + 1},     // Right
        {y - 1, x + 1}, // Down right
        {y - 1, x},     // Down
        {y - 1, x - 1}, // Down left
        {y, x - 1}};    // Left

    int neighbors = 0;

    for (int i = 0; i < 8; i++){

        int y = checkCoords[i][0];
        int x = checkCoords[i][1];

        if (x < 0){

            x = MAX_X_POS - 1;
        }
        else if (x > MAX_X_POS - 1){
            x = 0;
        }

        if (y < 0){
            y = MAX_Y_POS - 1;
        }
        else if (y > MAX_Y_POS - 1){
            y = 0;
        }

        if (mainMatrix[y][x] == CELL_CHAR){
            neighbors++;
        }
    }

    return neighbors;
}

/*
    Update each cell based on the amount of neighbors for that cell, as per the rules of the game
*/
void updateCells(){
    for (int i = MAX_Y_POS - 1; i > 0; i--){
        for (int j = 0; j < MAX_X_POS; j++){
            if (neighborCountBuffer[i][j] < 2 || neighborCountBuffer[i][j] > 3){
                mainMatrix[i][j] = BACKGROUND_CHAR;}
            else if (neighborCountBuffer[i][j] == 3){
                mainMatrix[i][j] = CELL_CHAR;
            }
        }
    }
}