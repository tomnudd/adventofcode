#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 128

int max(int a, int b)
{
    return a > b ? a : b;
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

    // open
    FILE *input = fopen(argv[1], "r");
    if (!input)
    {
        printf("Failed to open file: %s", argv[1]);
        return 1;
    }

    // read into arrs
    int **arr = NULL;
    int *columnCount = NULL;

    int lineCount = 0;
    char line[BUFFER_SIZE];
    char store[BUFFER_SIZE];
    while(fgets(line, sizeof(line), input) != NULL)
    {
        strncpy(store, line, BUFFER_SIZE);
        // How many numbers are there?
        int numCount = 0;
        char *v = strtok(line, " ");
        while (v != NULL)
        {
            numCount++;
            v = strtok(NULL, " "); // keep tokenizing
        }

        // Allocate memory for column count
        columnCount = (int *)realloc(columnCount, (lineCount+1) * sizeof(int));
        columnCount[lineCount] = numCount;

        // Allocate memory for 2d int array
        arr = (int **)realloc(arr, (lineCount+1) * sizeof(int *));
        arr[lineCount] = (int *)malloc(numCount * sizeof(int));

        // error checking todo

        // Populate array with ints
        int i = 0;
        v = strtok(store, " ");
        while (v != NULL)
        {
            arr[lineCount][i++] = atoi(v);
            v = strtok(NULL, " ");
        }

        lineCount++;
    }

    // Part 1
    int safeCount = 0;
    int i;
    for (i = 0; i < lineCount; i++)
    {
        int isUp = 0;
        int isDown = 0;
        int maxDiff = 0;

        int columns = columnCount[i];

        int j;
        for (j = 0; j < columns; j++)
        {
            int val = arr[i][j];

            // stop comparing if we are at the final element
            if (j+1 >= columns)
            {
                continue;
            }

            // >= and <= to catch equality case, which is a failure
            if (val >= arr[i][j+1])
            {
                isDown = 1;
            }
            if (val <= arr[i][j+1])
            {
                isUp = 1;
            }

            maxDiff = max(maxDiff, abs(arr[i][j] - arr[i][j+1]));
        }

        if (isUp != isDown && maxDiff < 4)
        {
            safeCount++;
        }
    }

    printf("PART 1: %d\n", safeCount);

    // Part 2

    // BETTER: we need an array of differences, then remove the one causing a failure, see if it works, then done

    safeCount = 0;
    for (i = 0; i < lineCount; i++)
    {
        int columns = columnCount[i];
        int jSkip; // start at -1 to cover the no-removal case
        for (jSkip = -1; jSkip < columns; jSkip++)
        {
            int isUp = 0;
            int isDown = 0;
            int maxDiff = 0;

            // now skipping jSkip;
            int j;
            for (j = 0; j < columns; j++)
            {
                int val = arr[i][j];
                int tmp = j+1;
                if (tmp == jSkip)
                {
                    tmp = j+2;
                }

                // we are a skipped element
                // or we want to compare with an element beyond the end
                if (j == jSkip || tmp >= columns)
                {
                    continue;
                }

                // >= and <= to catch equality case, which is a failure
                if (val >= arr[i][tmp])
                {
                    isDown = 1;
                }
                if (val <= arr[i][tmp])
                {
                    isUp = 1;
                }

                maxDiff = max(maxDiff, abs(arr[i][j] - arr[i][tmp]));
            }

            // found a removal that works!
            if (isUp != isDown && maxDiff < 4)
            {
                safeCount++;
                break;
            }

        }
    }

    printf("PART 2: %d\n", safeCount);

    // free
    free(columnCount);
    free(arr);

    return 0;

}
