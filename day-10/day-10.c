#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

struct pos 
{
    int i;
    int j;
};

void getSize(char *filename, int *cols, int *rows)
{
    // open
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    }

    (*cols) = 0;
    (*rows) = 0;

    int longest = -1;
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input) != NULL)
    {
        (*rows)++;
        int len = strcspn(line, "\n"); // count until newline
        if (len > longest) longest = len;
    }
    fclose(input);

    (*cols) = longest;
}

void loadData(int **arr, char *filename, int cols, int rows)
{
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    }

    // read into arr
    int i;
    for (i = 0; i < rows; i++)
    {
        char buf[BUFFER_SIZE];
        if (fgets(buf, cols+2, input) != NULL) // \n\0
        {
            int j;
            for (j = 0; j < cols; j++)
            {
                arr[i][j] = buf[j] - '0'; // convert char to int
            }
        }
    }

    fclose(input);
}

// check that a point has not already been added
// better: set
void addDistinctPoint(struct pos *points, int *pi, int i, int j)
{
    int p;
    for (p = 0; p < *pi; p++)
    {
        if (points[p].i == i && points[p].j == j)
        {
            return;
        }
    }

    if (*pi + 1 >= BUFFER_SIZE)
    {
        printf("Too many distinct points, increase BUFFER_SIZE.\n");
        exit(1);
    }

    points[*pi].i = i;
    points[*pi].j = j;
    (*pi)++;
}

// recursion with lots of parameters
void check(int **arr, int i, int j, int rows, int cols, struct pos *points, int *pi, int *score)
{
    int me = arr[i][j];
    if (me == 9)
    {
        // add point
        addDistinctPoint(points, pi, i, j); // part 1
        (*score)++; // part 2
    }

    if ((i - 1 >= 0) && (arr[i-1][j] == me + 1)) // up
    {
        check(arr, i-1, j, rows, cols, points, pi, score);
    }

    if ((i + 1 < rows) && (arr[i+1][j] == me + 1)) // down
    {
        check(arr, i+1, j, rows, cols, points, pi, score);
    }

    if ((j - 1 >= 0) && (arr[i][j-1] == me + 1)) // left
    {
        check(arr, i, j-1, rows, cols, points, pi, score);
    }

    if ((j + 1 < cols) && (arr[i][j+1] == me + 1)) // right
    {
        check(arr, i, j+1, rows, cols, points, pi, score);
    }
}

int main(int argc, char **argv)
{
    // check
    if (argc != 2)
    {
        printf("Only one parameter: input file path");
        return 1;
    }

    int cols;
    int rows;
    getSize(argv[1], &cols, &rows);

    // alloc
    int **arr = (int **)malloc(rows * sizeof(int *));
    int i;
    for (i = 0; i < rows; i++)
    {
        arr[i] = (int *)malloc((cols + 1) * sizeof(int)); // \0
    }

    // load
    loadData(arr, argv[1], cols, rows);

    // Part 1 and Part 2
    int part1 = 0;
    int part2 = 0;
    for (i = 0; i < rows; i++)
    {
        int j;
        for (j = 0; j < cols; j++)
        {
            if (arr[i][j] == 0)
            {
                struct pos points[BUFFER_SIZE];
                int pi = 0;
                check(arr, i, j, rows, cols, points, &pi, &part2);

                part1 += pi;
            }
        }
    }

    printf("Part 1: %d\n", part1);
    printf("Part 2: %d\n", part2);

    // free
    for (i = 0; i < rows; i++)
    {
        free(arr[i]);
    }
    free(arr);

    return 0;
}