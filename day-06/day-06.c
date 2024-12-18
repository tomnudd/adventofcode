#include "hashy.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 512

void getSize(char *filename, int *cols, int *lines)
{
    // open
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("Failed to open file: %s", filename);
        return;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input) != NULL)
    {
        (*lines)++;
        *cols = strcspn(line, "\n");
    }

    //close
    fclose(input);
}

void turn(int *x, int *y)
{
    if (*x == -1) // <  ==>  ^
    {
        *x = 0;
        *y = -1;
    }
    else if (*x == 1) // >  ==>  v
    {
        *x = 0;
        *y = 1;
    }
    else if (*y == -1) // ^  ==>  >
    {
        *x = 1;
        *y = 0;
    }
    else if (*y == 1) // v  =>  <
    {
        *x = -1;
        *y = 0;
    }
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

    int colCount = 0;
    int lineCount = 0;
    getSize(argv[1], &colCount, &lineCount);

    // allocations
    char **arr = (char **)malloc(colCount * sizeof(char *));
    
    int i;
    for (i = 0; i < lineCount; i++)
    {
        arr[i] = (char *)malloc((colCount + 1) * sizeof(char)); // \0
    }

    // read into arrs
    FILE *input = fopen(argv[1], "r");
    if (!input)
    {
        printf("Failed to open file: %s", argv[1]);
        return 1;
    }

    for (i = 0; i < lineCount; i++)
    {
        if (fgets(arr[i], colCount+2, input) != NULL) // \n\0
        {
            arr[i][colCount] = '\0'; // \n -> \0
        }
    }

    fclose(input);

    // search for initial ^ < > or v (could be done above)
    char c;
    int idx = -1;
    int line = -1;
    for (i = 0; i < colCount; i++)
    {
        char *pos = strpbrk(arr[i], "^<>v");
        if (pos)
        {
            idx = (int)(pos - arr[i]);
            line = i;
            c = *pos;
        }
    }

    int x = 0;
    int y = 0;
    if (c == '^')
    {
        y = -1;
    }
    else if (c == '<')
    {
        x = -1;
    }
    else if (c == '>')
    {
        x = 1;
    }
    else if (c == 'v')
    {
        y = 1;
    }

    // store for later use
    int startLine = line;
    int startIdx = idx;
    int startY = y;
    int startX = x;

    struct hashy *visited[ARRAY_SIZE] = {NULL};
    char problems[ARRAY_SIZE][7];

    // iterate until we escape the lab
    int moveCount = 1; // factor in move #1
    int pc = 0;
    while (idx >= 0 && line >= 0 && idx < colCount && line < lineCount)
    {
        char buf[BUFFER_SIZE];
        snprintf(buf, sizeof(buf), "%03d%03d", line, idx);
        add(visited, buf, 1);
        memset(buf, 0, sizeof(buf));

        // are we moving out?
        if (idx + x < 0 || line + y < 0 || idx + x >= colCount || line + y >= lineCount)
        {
            break;
        }

        // cur pos is arr[line][idx];
        if (arr[line + y][idx + x] == '#')
        {
            turn(&x, &y);
        }
        else
        {
            // count whether the next move will contribute to the move count
            // does not catch move 1
            snprintf(buf, sizeof(buf), "%03d%03d", line + y, idx + x);
            if ((arr[line + y][idx + x] == '.') && lookup(visited, buf) == NULL)
            {
                moveCount++;
                strncpy(problems[pc], buf, 6);
                problems[pc][6] = '\0';
                pc++;
            }

            line += y;
            idx += x;
        }
    }
    freeHashy(visited);

    printf("PART 1: %d\n", moveCount);

    // PART 2
    // Track direction as well
    int cycleCount = 0;
    for (i = 0; i < pc; i++)
    {
        int blockX;
        int blockY;
        if (sscanf(problems[i], "%03d%03d", &blockY, &blockX) != 2)
        {
            return 1;
        }

        // initialise new hash table and restart from the top
        struct hashy *cycleVisited[ARRAY_SIZE] = {NULL};
        x = startX;
        y = startY;
        line = startLine;
        idx = startIdx;

        // as before, but storing direction as well
        while (idx >= 0 && line >= 0 && idx < colCount && line < lineCount)
        {
            char buf[BUFFER_SIZE];
            snprintf(buf, sizeof(buf), "%03d%03d%1d%1d", line, idx, y, x);
            if (lookup(cycleVisited, buf) != NULL)
            {
                cycleCount++;
                break;
            }

            add(cycleVisited, buf, 1);
            memset(buf, 0, sizeof(buf));

            // are we moving out?
            if (idx + x < 0 || line + y < 0 || idx + x >= colCount || line + y >= lineCount)
            {
                break;
            }

            // blockX and blockY considered as obstacles too
            if (arr[line + y][idx + x] == '#' || (line + y == blockY && idx + x == blockX))
            {
                turn(&x, &y);
            }
            else
            {
                line += y;
                idx += x;
            }
        }
        freeHashy(cycleVisited);
    }

    printf("PART 2: %d\n", cycleCount);

    // free
    for (i = 0; i < lineCount; i++)
    {
        free(arr[i]);
    }
    free(arr);

    return 0;
}