#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 5000

// Based on hashmap in day-01
struct hashy {
    int key;
    int *val;
    int count;
    int orig;
    struct hashy *next;
};

int hash(int i)
{
    return (i * 31) % ARRAY_SIZE;
}

struct hashy *lookup(struct hashy **tab, int i)
{
    struct hashy *p;
    for (p = tab[hash(i)]; p != NULL; p = p->next)
    {
        if (i == p->key)
        {
            return p;
        }
    }
    return NULL;
}

struct hashy *add(struct hashy **tab, int key, int val, int orig)
{
    struct hashy *p;
    int hashValue;

    p = lookup(tab, key);
    if (p == NULL)
    {
        // new item
        p = (struct hashy *) malloc (sizeof(*p)); // never freed

        p->key = key;
        hashValue = hash(key);
        p->next = tab[hashValue];

        p->val = (int *) malloc(sizeof(int)); // initially 1 slot
        p->val[0] = val;
        p->count = 1;

        p->orig = orig;

        tab[hashValue] = p;
    }
    else
    {
        int sz = p->count;
        sz++;
        p->val = realloc(p->val, sz * sizeof(int));
        p->val[sz-1] = val;
        p->count = sz;

        p->orig = orig;
    }

    return p;
}

void freeHashy(struct hashy **tab)
{
    int i;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        struct hashy *cur = tab[i];
        while (cur != NULL)
        {
            struct hashy *next = cur->next;

            if (cur->val != NULL)
            {
                free(cur->val);
            }

            free(cur);

            cur = next;
        }
    }
}

int checkArr(struct hashy **tab, int *z, int len, int *fi, int *fo)
{
    struct hashy *temp[ARRAY_SIZE] = {NULL};

    int p1 = 1;
    int i;
    for (i = 0; i < len; i++)
    {
        int val = z[i];
        struct hashy *t = lookup(temp, val);
        if (t != NULL)
        {
            p1 = 0;
            *fi = i;
            *fo = t->orig;
        }

        // Add the values we cannot now see (given val has been seen) to our hash
        struct hashy *u = lookup(tab, val);
        if (u != NULL)
        {
            int j;
            for (j = 0; j < u->count; j++)
            {
                add(temp, u->val[j], 1, i);
            }
        }
    }

    freeHashy(temp);

    return p1;
}

// If we have seen 47|53, this means 47 must be before 53.
// Therefore, if we see 53, we cannot see 47 later
// 53 is the key, its values cannot be seen later
void processLine(struct hashy **tab, char *line, int *part1, int *part2)
{
    int z[64] = {0};

    // for thing we see
        // return 0 if in local hash
        // add ->val to our local hash
    int len = 0;
    char *tok;
    tok = strtok(line, ",");
    while (tok != NULL)
    {
        int val;
        if (sscanf(tok, "%d", &val) == 1)
        {
            z[len] = val;
            len++;
        }
        tok = strtok(NULL, ",");
    }

    int fi = -1;
    int fo = -1;
    int p1 = 1;
    p1 = checkArr(tab, z, len, &fi, &fo);

    if (p1)
    {
        *part1 += z[len/2];
        *part2 += 0;
        return;
    }

    // track the following:
        // fi: index of problem
        // fo: index of origin of problem (what caused the element at z[fi] to fail)
    // idea: swap these elements in z until it works
    while (!p1)
    {
        int tmp = z[fi];
        z[fi] = z[fo];
        z[fo] = tmp;

        fi = -1;
        fo = -1;
        p1 = checkArr(tab, z, len, &fi, &fo);
    }

    *part2 += z[len/2];
}

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

    struct hashy *table[ARRAY_SIZE] = {NULL};

    int part1 = 0;
    int part2 = 0;

    // read and parse
    char line[128];
    int rules = 1;
    while (fgets(line, sizeof(line), input) != NULL)
    {
        if (strcmp(line, "\n") == 0)
        {
            rules = 0;
            continue;
        }

        if (rules)
        {
            int p1;
            int p2;
            if (sscanf(line, "%d|%d", &p1, &p2) == 2)
            {
                // Add multiplication to sum
                add(table, p2, p1, 0);
            }
        }
        else
        {
            processLine(table, line, &part1, &part2);

            // alternative ideas: (1) build graph then tsort, (2) qsort z arrays based on rules
        }

    }

    // close
    fclose(input);


    printf("PART 1: %d\n", part1);
    printf("PART 2: %d\n", part2);


}