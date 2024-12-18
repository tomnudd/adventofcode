#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

void getSize(char *filename, int *ints, int *lines)
{
    // open
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    int chars = 0;
    int max = -1;
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input) != NULL)
    {
        (*lines)++;
        chars = strcspn(line, "\n");

        // count numbers on line
        int c = 0;
        int inNum = 0;
        int i;
        for (i = 0; i < chars; i++)
        {
            if (isdigit(line[i]))
            {
                if (!inNum)
                {
                    c++;
                    inNum = 1;
                }
            }
            else
            {
                inNum = 0;
            }
        }

        if (c > max)
        {
            max = c;
        }

    }
    fclose(input);

    (*ints) = max;
}

void concat(unsigned long long *result, unsigned long long n1, unsigned long long n2)
{
    // Concatenate

    //slow way:
    //char temp[BUFFER_SIZE];
    //sprintf(temp, "%llu%llu", n1, n2);
    //*result = strtoull(temp, NULL, 10);

    // count digits of n2, shift n1 by this much, then add n2
    int digits = 0;
    unsigned long long temp = n2;

    while (temp > 0)
    {
        temp /= 10;
        digits++;
    }

    int pow = 1;
    int i;
    for (i = 0; i < digits; i++)
    {
        pow *= 10;
    }

    *result = n1 * pow + n2;
}

// Part 1: verify if target can be reached with + and *
int verify(unsigned long long target, unsigned long long *nums, unsigned long long cur, int size, int pos)
{
    if (cur == target && pos == size) return 1;
    if (pos > size || cur > target) return 0;
    
    if (verify(target, nums, cur + nums[pos], size, pos+1))
    {
        return 1;
    }
    if (verify(target, nums, cur * nums[pos], size, pos+1))
    {
        return 1;
    }

    return 0;
}

// Part 2: verify if target can be reached with +, *, and ||
int verify2(unsigned long long target, unsigned long long *nums, unsigned long long cur, int size, int pos)
{
    if (cur == target && pos == size) return 1;
    if (pos > size || cur > target) return 0;
    
    if (verify2(target, nums, cur + nums[pos], size, pos+1))
    {
        return 1;
    }
    if (verify2(target, nums, cur * nums[pos], size, pos+1))
    {
        return 1;
    }

    unsigned long long combo;
    concat(&combo, cur, nums[pos]);
    if (verify2(target, nums, combo, size, pos+1))
    {
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    // check
    if (argc != 2)
    {
        printf("Only one parameter: input file path");
        return 1;
    }

    // get size
    int maxNums = 0;
    int lines = 0;
    getSize(argv[1], &maxNums, &lines);

    // open
    FILE *input = fopen(argv[1], "r");
    if (!input)
    {
        printf("Failed to open file: %s\n", argv[1]);
        return 1;
    }

    // llu required with numbers this big
    unsigned long long part1 = 0;
    unsigned long long part2 = 0;

    // read file
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), input) != NULL)
    {
        // get target
        unsigned long long target = 0;
        if (sscanf(line, "%llu:", &target) != 1)
        {
            printf("Weird line found...\n");
            return 1;
        }

        char *p = strchr(line, ':');
        if (p == NULL)
        {
            printf("Weird line found...\n");
            return 1;
        }
        p+=2; // skip : and space

        // malloc and read numbers
        unsigned long long *nums = (unsigned long long *)malloc(maxNums * sizeof(unsigned long long));
        int i = 0;
        while (sscanf(p, "%llu ", &nums[i]) == 1)
        {
            i++;

            // move past space
            p = strchr(p, ' ');
            if (p == NULL)
            {
                break;
            }
            p++;
        }

        // Part 1
        if (verify(target, nums, 0, i, 0))
        {
            part1 += target;
        }

        // Part 2
        if (verify2(target, nums, 0, i, 0))
        {
            part2 += target;
        }

        free(nums);
    }
    fclose(input);

    printf("PART 1: %llu\n", part1);
    printf("PART 2: %llu\n", part2);

    return 0;
}