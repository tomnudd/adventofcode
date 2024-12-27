#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashy.h"

#define BUFFER_SIZE 512
#define ITERATIONS_1 25
#define ITERATIONS_2 75

int countDigits(unsigned long long n)
{
    int digits = 0;

    while (n > 0)
    {
        n /= 10;
        digits++;
    }

    return digits;
}

unsigned long long count(unsigned long long stone, int iters, struct hashy **memory)
{
    // base
    if (iters == 0) return 1;

    // memory
    // key is "stone,iters"
    char key[128];
    snprintf(key, sizeof(key), "%llu,%d", stone, iters);
    struct hashy *mem = lookup(memory, key);
    if (mem != NULL)
    {
        return mem->val;
    }

    unsigned long long c = 0;
    if (stone == 0)
    {
        // 0 becomes 1
        c = count(1, iters - 1, memory);
    }
    else
    {
        int digits = countDigits(stone);
        if (digits % 2 == 0)
        {
            // even number of digits: left half on one, right half on another
            unsigned long long pow = 1;
            int i;
            for (i = 0; i < digits / 2; i++)
            {
                pow *= 10;
            }

            // 12345678 / 10000
            unsigned long long s1 = stone / pow;

            // 12345678 % 10000
            unsigned long long s2 = stone % pow;

            c = count(s1, iters - 1, memory) + count(s2, iters - 1, memory);
        }
        else
        {
            // odd number of digits: multiply by 2024
            c = count(stone * 2024, iters - 1, memory);
        }
    }

    // hash and return the result
    add(memory, key, c);
    return c;
}

int main(int argc, char **argv)
{
    // check
    if (argc != 2)
    {
        printf("Only one parameter: input file path");
        exit(1);
    }

    // open
    FILE *input = fopen(argv[1], "r");
    if (!input)
    {
        printf("Failed to open file: %s\n", argv[1]);
        exit(1);
    }
    
    // read in
    unsigned long long arr[BUFFER_SIZE];
    int c = 0;
    while (fscanf(input, "%llu ", &arr[c]) == 1)
    {
        c++;
    }

    // close
    fclose(input);

    // added for part 2: hash table to remember results
    struct hashy *remember[ARRAY_SIZE] = {NULL};

    // Part 1
    unsigned long long part1 = 0;
    // For each stone seen, iterate and count how many stones it results in
    int i;
    for (i = 0; i < c; i++)
    {
        part1 += count(arr[i], ITERATIONS_1, remember);
    }

    printf("PART 1: %llu\n", part1);

    // Part 2
    unsigned long long part2 = 0;
    for (i = 0; i < c; i++)
    {
        part2 += count(arr[i], ITERATIONS_2, remember);
    }

    printf("PART 2: %llu\n", part2);

    freeHashy(remember);

    return 0;
}