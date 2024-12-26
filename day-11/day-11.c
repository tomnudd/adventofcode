#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

int count(unsigned long long stone, int iters)
{
    if (iters == 0) return 1; // base

    // 0 becomes 1
    if (stone == 0)
    {
        return count(1, iters - 1);
    }
    
    int digits = countDigits(stone);
    if (digits % 2 == 0)
    {
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

        return count(s1, iters - 1) + count(s2, iters - 1);
    }
    else
    {
        return count(stone * 2024, iters - 1);
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

    FILE *input = fopen(argv[1], "r");
    if (!input)
    {
        printf("Failed to open file: %s\n", argv[1]);
        exit(1);
    }
    
    unsigned long long arr[BUFFER_SIZE];
    int c = 0;
    while (fscanf(input, "%llu ", &arr[c]) == 1)
    {
        c++;
    }

    fclose(input);

    // Part 1
    int part1 = 0;
    // For each stone seen, iterate and count how many stones it results in
    int i;
    for (i = 0; i < c; i++)
    {
        part1 += count(arr[i], ITERATIONS_1);
    }

    printf("PART 1: %d\n", part1);

    // Part 2
    /*int part2 = 0;
    // For each stone seen, iterate and count how many stones it results in
    for (i = 0; i < c; i++)
    {
        part1 += count(arr[i], ITERATIONS_2);
    }

    printf("PART 2: %d\n", part2);*/

    return 0;
}