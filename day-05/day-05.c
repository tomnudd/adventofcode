#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 5000

// Based on hashmap in day-01
struct hashy {
    int key;
    int *val;
    int count;
    struct hashy *next;
};

struct hashy *table[ARRAY_SIZE] = {NULL};

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

struct hashy *add(struct hashy **tab, int key, int val)
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

        tab[hashValue] = p;
    }
    else
    {
        int sz = p->count;
        sz++;
        p->val = realloc(p->val, sz * sizeof(int));
        p->val[sz-1] = val;
        p->count = sz;
    }

    return p;
}

// If we have seen 47|53, this means 47 must be before 53.
// Therefore, if we see 53, we cannot see 47 later
// 53 is the key, its values cannot be seen later
int processLine(char *line)
{
    struct hashy *temp[ARRAY_SIZE] = {NULL};
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
            if (lookup(temp, val))
            {
                return 0;
            }

            z[len] = val;
            len++;
            
            // Add the values we cannot now see (given val has been seen) to our hash
            struct hashy *u = lookup(table, val);
            if (u != NULL)
            {
                int i;
                for (i = 0; i < u->count; i++)
                {
                    add(temp, u->val[i], 1);
                }
            }
        }
        tok = strtok(NULL, ",");
    }

    return z[len/2];
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

    int count = 0;
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
                add(table, p2, p1);
            }
        }
        else
        {
            count += processLine(line);

            // part 2 = tsort?
        }

    }

    printf("PART 1: %d\n", count);

    fclose(input);

}