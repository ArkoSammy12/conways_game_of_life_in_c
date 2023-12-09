#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_X_POS 100
#define MAX_Y_POS 35

const char BACKGROUND_CHAR = ' ';
const char CELL_CHAR = '#';

char mainMatrix[MAX_Y_POS][MAX_X_POS];
char neighborCountBuffer[MAX_Y_POS][MAX_X_POS]; // Keep track of the amount of neighbors that each cell has separately

struct Cell
{
    int x;
    int y;
};

void initMatrix();
void setInitialCells(struct Cell c[], size_t);
void conwayLoop();
void updateCells();
void printCells();
int countNeighbors(int, int);

int main()
{
    int initialCellsCount = 0;
    bool exit = false;

    struct Cell pointList[MAX_Y_POS * MAX_X_POS];

    while (!exit)
    {
        struct Cell p;

        int xCoord = 0;
        int yCoord = 0;

        printf_s("\nInput the x and y coordinates of cell #%d: \n", (initialCellsCount + 1));
        scanf("%d %d", &xCoord, &yCoord);

        p.x = xCoord;
        p.y = yCoord;

        pointList[initialCellsCount] = p;
        initialCellsCount++;

        fflush(stdin);

        bool subExit = false;

        if (initialCellsCount >= MAX_Y_POS * MAX_X_POS)
        {
            exit = true;
            subExit = true;
        }

        while (!subExit)
        {
            char opt;
            printf("\n\nDo you want to input more cells? y/n\n");
            scanf(" %c", &opt);

            fflush(stdin);

            switch (opt)
            {
            case 'Y':
            case 'y':
                subExit = true;
                break;
            case 'n':
            case 'N':
                exit = true;
                subExit = true;
                break;
            default:
                printf("Invalid option\n");
            }
        }
    }

    initMatrix();
    setInitialCells(pointList, initialCellsCount);
    conwayLoop();

    return 0;
}

void conwayLoop()
{

    while (true)
    {
        system("cls");
        printCells();
        updateCells();
        sleep(1);
    }
}

void initMatrix()
{
    for (int i = 0; i < MAX_Y_POS; i++)
    {
        for (int j = 0; j < MAX_X_POS; j++)
        {
            mainMatrix[i][j] = BACKGROUND_CHAR;
        }
    }
}

/*
    Place initial alive cells in the matrix
*/
void setInitialCells(struct Cell cellList[], size_t size)
{

    for (int i = 0; i < size; i++)
    {
        struct Cell c = cellList[i];
        mainMatrix[c.y][c.x] = CELL_CHAR;
    }
}

void printCells()
{
    for (int i = MAX_Y_POS - 1; i >= 0; i--)
    {
        for (int j = 0; j < MAX_X_POS; j++)
        {
            printf_s("%c", mainMatrix[i][j]);
            neighborCountBuffer[i][j] = countNeighbors(i, j);
        }
        printf_s("\n");
    }
}

/*
    Count surrounding cells for alive neighbors
*/
int countNeighbors(int y, int x)
{
    int yStart = y + 1;
    int yEnd = y - 1;
    int xStart = x - 1;
    int xEnd = x + 1;

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

    for (int i = 0; i < 8; i++)
    {

        int y = checkCoords[i][0];
        int x = checkCoords[i][1];

        if (x < 0)
        {

            x = MAX_X_POS - 1;
        }
        else if (x > MAX_X_POS - 1)
        {
            x = 0;
        }

        if (y < 0)
        {
            y = MAX_Y_POS - 1;
        }
        else if (y > MAX_Y_POS - 1)
        {
            y = 0;
        }

        if (mainMatrix[y][x] == CELL_CHAR)
        {
            neighbors++;
        }
    }

    return neighbors;
}

/*
    Update each cell based on the amount of neighbors for that cell, as per the rules of the game
*/
void updateCells()
{
    for (int i = MAX_Y_POS - 1; i > 0; i--)
    {
        for (int j = 0; j < MAX_X_POS; j++)
        {
            if (neighborCountBuffer[i][j] < 2 || neighborCountBuffer[i][j] > 3)
            {
                mainMatrix[i][j] = BACKGROUND_CHAR;
            }
            else if (neighborCountBuffer[i][j] == 3)
            {
                mainMatrix[i][j] = CELL_CHAR;
            }
        }
    }
}
