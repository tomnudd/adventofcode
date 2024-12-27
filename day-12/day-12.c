#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

struct element
{
    char type;
    int contribution;
    int region;
};

struct regionElement
{
    int count;
    int score;
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

void loadData(struct element **arr, char *filename, int cols, int rows)
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
                arr[i][j].type = buf[j];
                arr[i][j].contribution = -1;
                arr[i][j].region = -1;
            }
        }
    }

    fclose(input);
}

void explore(struct element **arr, int i, int j, int rows, int cols, int *region)
{
    if (arr[i][j].region != -1) return;

    arr[i][j].region = *region;

    char type = arr[i][j].type;

    if (i >= 1 && arr[i-1][j].type == type) // up
    {
        explore(arr, i-1, j, rows, cols, region);
    }

    if (i + 1 < rows && arr[i+1][j].type == type) // down
    {
        explore(arr, i+1, j, rows, cols, region);
    }

    if (j >= 1 && arr[i][j-1].type == type) // left
    {
        explore(arr, i, j-1, rows, cols, region);
    }

    if (j + 1 < cols && arr[i][j+1].type == type) // right
    {
        explore(arr, i, j+1, rows, cols, region);
    }
}

void score(struct element **arr, int i, int j, int rows, int cols)
{
    char type = arr[i][j].type;

    int s = 0;
    if (i < 1 || arr[i-1][j].type != type) s++; // up
    if (i + 1 >= rows || arr[i+1][j].type != type) s++; // down
    if (j < 1 || arr[i][j-1].type != type) s++; // left
    if (j + 1 >= cols || arr[i][j+1].type != type) s++; // right

    arr[i][j].contribution = s;
}

int main(int argc, char **argv)
{
    // check
    if (argc != 2)
    {
        printf("Only one parameter: input file path");
        exit(1);
    }

    int cols;
    int rows;
    getSize(argv[1], &cols, &rows);

    // alloc
    struct element **arr = (struct element **)malloc(rows * sizeof(struct element *));
    int i;
    for (i = 0; i < rows; i++)
    {
        arr[i] = (struct element *)malloc((cols + 1) * sizeof(struct element));
    }

    // load
    loadData(arr, argv[1], cols, rows);

    // individually score each element for its perimeter contribution
    // explore the region surrounding each element if missing a region
    int region = 0;
    for (i = 0; i < rows; i++)
    {
        int j;
        for (j = 0; j < cols; j++)
        {
            score(arr, i, j, rows, cols);

            if (arr[i][j].region != -1) continue;
            explore(arr, i, j, rows, cols, &region);
            region++;
        }
    }

    // alloc regions
    struct regionElement *regions = (struct regionElement *)malloc(region * sizeof(struct regionElement));
    for (i = 0; i < region; i++)
    {
        regions[i].count = 0;
        regions[i].score = 0;
    }

    // sum individual contributions and count size of region
    for (i = 0; i < rows; i++)
    {
        int j;
        for (j = 0; j < cols; j++)
        {
            int r = arr[i][j].region;
            int s = arr[i][j].contribution;
            
            regions[r].count++;
            regions[r].score += s;
        }
    }

    // Part 1
    int part1 = 0;
    for (i = 0; i < region; i++)
    {
        part1 += regions[i].count * regions[i].score;
        //printf("Region %d has count %d, score %d, so res %d\n", i, regions[i].count, regions[i].score, regions[i].count * regions[i].score);
    }
    printf("Part 1: %d\n", part1);

    // free
    free(regions);
    for (i = 0; i < rows; i++)
    {
        free(arr[i]);
    }
    free(arr);

    return 0;
}