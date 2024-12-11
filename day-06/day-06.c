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

    // iterate until we escape the lab
    int moveCount = 1; // factor in move #1
    while (idx >= 0 && line >= 0 && idx < colCount && line < lineCount)
    {
        arr[line][idx] = 'X';

        // cur pos is arr[line][idx];
        if (arr[line + y][idx + x] == '#')
        {
            if (x == -1) // <  ==>  ^
            {
                x = 0;
                y = -1;
            }
            else if (x == 1) // >  ==>  v
            {
                x = 0;
                y = 1;
            }
            else if (y == -1) // ^  ==>  >
            {
                x = 1;
                y = 0;
            }
            else if (y == 1) // v  =>  <
            {
                x = -1;
                y = 0;
            }
        }
        else
        {
            // are we moving out?
            if (idx + x < 0 || line + y < 0 || idx + x >= colCount || line + y >= lineCount)
            {
                break;
            }

            // count whether the next move will contribute to the move count
            // does not catch move 1
            if (arr[line + y][idx + x] == '.')
            {
                moveCount++;
            }

            line = line + y;
            idx = idx + x;
        }
    }

    printf("PART 1: %d\n", moveCount);

    // free
    for (i = 0; i < lineCount; i++)
    {
        free(arr[i]);
    }
    free(arr);

    return 0;
}