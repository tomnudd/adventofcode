#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 512

struct point
{
    int x;
    int y;
};

void getSize(char *filename, int *cols, int *lines)
{
    // open
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("Failed to open file: %s", filename);
        return;
    }

    // count until newline; all lines have same length
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input) != NULL)
    {
        (*lines)++;
        *cols = strcspn(line, "\n");
    }

    fclose(input);
}

void loadData(char **arr, char *filename, int cols, int lines)
{
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    int i;
    for (i = 0; i < lines; i++)
    {
        arr[i] = (char *)malloc((cols + 1) * sizeof(char)); // \0
    }

    // read into arrs
    for (i = 0; i < lines; i++)
    {
        if (fgets(arr[i], cols+2, input) != NULL) // \n\0
        {
            arr[i][cols] = '\0'; // \n -> \0
        }
    }

    fclose(input);
}

void getPoints(struct point **points, char **arr, int cols, int lines, char symbol, int *pc)
{
    // find the points with given symbol and add to points
    int i;
    for (i = 0; i < lines; i++)
    {
        int j;
        for (j = 0; j < cols; j++)
        {
            if (arr[i][j] != symbol) continue;
            
            // realloc
            *points = (struct point *)realloc(*points, (*pc + 1) * sizeof(struct point));

            // add new point
            struct point p;
            p.x = j;
            p.y = i;

            (*points)[*pc] = p;
            (*pc)++;
        }
    }
}

void addDistinctPoint(struct point **distinct, int x, int y, int *pc)
{
    // lazy, but check all points to see if it already exists
    // set would be better
    int i;
    for (i = 0; i < *pc; i++)
    {
        if ((*distinct)[i].x == x && (*distinct)[i].y == y) return;
    }

    // realloc
    *distinct = (struct point *)realloc(*distinct, (*pc + 1) * sizeof(struct point));

    // add new point
    struct point p;
    p.x = x;
    p.y = y;

    (*distinct)[*pc] = p;
    (*pc)++;
}

// Main
int main(int argc, char **argv)
{
    // check
    if (argc != 2)
    {
        printf("Only one parameter: input file path");
        return 1;
    }

    int cols = 0;
    int lines = 0;
    getSize(argv[1], &cols, &lines);

    // allocations and loading
    char **arr = (char **)malloc(lines * sizeof(char *));
    loadData(arr, argv[1], cols, lines);
    
    // build array of symbols
    int symbols[BUFFER_SIZE] = {0};
    int i;
    for (i = 0; i < lines; i++)
    {
        int j;
        for (j = 0; j < cols; j++)
        {
            if (arr[i][j] != '.') symbols[arr[i][j]] = 1;
        }
    }

    // Part 1
    // for each symbol, find the points
    int distinctCount = 0;
    struct point *distinctPoints = NULL;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (symbols[i] != 1) continue;
        
        char symbol = i;

        // find things with symbol
        int pointsCount = 0;
        struct point *points = NULL;
        getPoints(&points, arr, cols, lines, symbol, &pointsCount);

        // pointsCount choose 2
        int j;
        for (j = 0; j < pointsCount; j++)
        {
            int xj = points[j].x;
            int yj = points[j].y;

            int k;
            for (k = j + 1; k < pointsCount; k++)
            {
                int xk = points[k].x;
                int yk = points[k].y;

                // first direction
                int newx = 2 * xk - xj;
                int newy = 2 * yk - yj;

                if (newx >= 0 && newy >= 0 && newx < cols && newy < lines)
                {
                    addDistinctPoint(&distinctPoints, newx, newy, &distinctCount);
                }

                // second direction
                newx = 2 * xj - xk;
                newy = 2 * yj - yk;

                if (newx >= 0 && newy >= 0 && newx < cols && newy < lines)
                {
                    addDistinctPoint(&distinctPoints, newx, newy, &distinctCount);
                }

            }
        }
        free(points);

    }
    free(distinctPoints);

    // Part 2
    int distinctCount2 = 0;
    struct point *distinctPoints2 = NULL;
    int iters = (lines > cols) ? lines : cols;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (symbols[i] != 1) continue;
        
        char symbol = i;

        // find things with symbol
        int pointsCount = 0;
        struct point *points = NULL;
        getPoints(&points, arr, cols, lines, symbol, &pointsCount);

        // pointsCount choose 2
        int j;
        for (j = 0; j < pointsCount; j++)
        {
            int xj = points[j].x;
            int yj = points[j].y;

            int k;
            for (k = j + 1; k < pointsCount; k++)
            {
                int xk = points[k].x;
                int yk = points[k].y;

                // consider z * distance in both directions, for z from 1 to iters
                // note that (a, b) and (c, d) now create antinodes (a, b) and (c, d)
                int z;
                for (z = 1; z < iters; z++)
                {
                    // first direction
                    int newx = xj + z * (xk - xj);
                    int newy = yj + z * (yk - yj);

                    if (newx >= 0 && newy >= 0 && newx < cols && newy < lines)
                    {
                        addDistinctPoint(&distinctPoints2, newx, newy, &distinctCount2);
                    }

                    // second direction
                    newx = xk + z * (xj - xk);
                    newy = yk + z * (yj - yk);

                    if (newx >= 0 && newy >= 0 && newx < cols && newy < lines)
                    {
                        addDistinctPoint(&distinctPoints2, newx, newy, &distinctCount2);
                    }
                }
            }
        }
        free(points);
    }
    free(distinctPoints2);
    free(arr);
    
    printf("PART 1: %d\n", distinctCount);
    printf("PART 2: %d\n", distinctCount2);

    return 0;
}