#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 32768

void readInput(char *filename, char *line, int *length, int *sum)
{
    // open
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("Failed to open file: %s", filename);
        return;
    }

    // count until newline
    if (fgets(line, BUFFER_SIZE, input) != NULL)
    {
        *length = strcspn(line, "\n");
        int i;
        for (i = 0; i < *length; i++)
        {
            *sum += (line[i] - '0');
        }
    }

    fclose(input);
}

int main(int argc, char **argv)
{
    // check
    if (argc != 2)
    {
        printf("Only one parameter: input file path");
        return 1;
    }

    int length = 0;
    int sum = 0;
    char line[BUFFER_SIZE] = "";
    readInput(argv[1], line, &length, &sum);

    // set up input array
    int *arr;
    arr = (int*)malloc(sum * sizeof(int));
    int i;
    for (i = 0; i < sum; i++)
    {
        arr[i] = -1;
    }

    // represent blocks (-1 for .)
    // even positions are files, odd positions are free space
    int id = 0;
    int pos = 0;
    for (i = 0; i < length; i++)
    {
        int c = line[i] - '0';
        if (i % 2 == 0)
        {
            int j;
            for (j = 0; j < c; j++)
            {
                arr[pos++] = id;
            }
            id++;
        }
        else
        {
            pos += c;
        }
    }

    // meet in the middle
    int l = 0;
    int r = sum - 1;
    while (l < r)
    {
        if (arr[r] == -1)
        {
            r--;
        }
        else if (arr[l] != -1)
        {
            l++;
        }
        else
        {
            arr[l] = arr[r];
            arr[r] = -1;
        }
    }

    // Part 1
    unsigned long long part1 = 0;
    for (i = 0; i < sum; i++)
    {
        if (arr[i] == -1) continue;
        part1 += (i * arr[i]);
    }

    printf("PART 1: %llu\n", part1);

    return 0;
}