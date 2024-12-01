#include <stdlib.h>
#include <stdio.h>

#define ARRAY_SIZE 1000

int compare(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

// Hashmap things!
struct hashy {
    int key;
    int val;
    struct hashy *next;
};

struct hashy *table[ARRAY_SIZE] = {NULL};

int hash(int i)
{
    return (i * 31) % ARRAY_SIZE;
}

struct hashy *lookup(int i)
{
    struct hashy *p;
    for (p = table[hash(i)]; p != NULL; p = p->next)
    {
        if (i == p->key)
        {
            return p;
        }
    }
    return NULL;
}

struct hashy *add(int key, int val)
{
    struct hashy *p;
    int hashValue;

    p = lookup(key);
    if (p == NULL)
    {
        // new item
        p = (struct hashy *) malloc (sizeof(*p)); // never freed

        p->key = key;
        hashValue = hash(key);
        p->next = table[hashValue];

        table[hashValue] = p;
    }

    p->val = val;
    return p;
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

    // read
    int arr1[ARRAY_SIZE];
    int arr2[ARRAY_SIZE];

    int i = 0;
    while(fscanf(input, "%d   %d", &arr1[i], &arr2[i]) == 2)
    {
        i++;
    }

    // close
    fclose(input);

    // sort
    qsort(arr1, ARRAY_SIZE, sizeof(int), compare);
    qsort(arr2, ARRAY_SIZE, sizeof(int), compare);

    // Part 1
    int sum = 0;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        sum += abs(arr1[i] - arr2[i]);
    }
    printf("PART 1: %d\n", sum);

    // Part 2
    int counts[100000] = {0}; // laziest solution
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        counts[arr2[i]]++;
    }

    sum = 0;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        sum += arr1[i] * counts[arr1[i]];
    }
    printf("PART 2: %d\n", sum);

    // Part 2: hashmap solution
    int curVal;
    struct hashy *temp;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        // table
        curVal = arr2[i];
        temp = lookup(curVal);
        if (temp == NULL)
        {
            add(curVal, 1);
        }
        else
        {
            temp->val++;
        }
    }

    sum = 0;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        curVal = arr1[i];
        temp = lookup(curVal);
        if (temp != NULL)
        {
            sum += curVal * temp->val;
        }

    }
    printf("PART 2.2: %d\n", sum);

    return 0;
}
