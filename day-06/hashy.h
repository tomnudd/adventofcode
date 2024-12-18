#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define ARRAY_SIZE 20000

struct hashy {
    char key[16];
    int val;
    struct hashy *next;
};

int hash(char *str)
{
    // djb2 Dan Bernstein
    unsigned long h = 5381;

    int c;
    while (c = *str++)
    {
        h = ((h << 5) + h) + c;
    }

    return h % ARRAY_SIZE;
}

struct hashy *lookup(struct hashy **tab, char *str)
{
    struct hashy *p;
    for (p = tab[hash(str)]; p != NULL; p = p->next)
    {
        if (strcmp(str, p->key) == 0)
        {
            return p;
        }

    }
    return NULL;
}

struct hashy *add(struct hashy **tab, char *key, int val)
{
    struct hashy *p;
    int hashValue;

    p = lookup(tab, key);
    if (p == NULL)
    {
        // new item
        p = (struct hashy *) malloc (sizeof(*p));

        strcpy(p->key, key);
        hashValue = hash(key);
        p->val = val;
        p->next = tab[hashValue];

        tab[hashValue] = p;
    }
    else
    {
        // "update"
        p->val = val;
    }

    return p;
}

// follow next pointers where possible, freeing everything in the hash table
void freeHashy(struct hashy **tab)
{
    int i;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        struct hashy *cur = tab[i];
        while (cur != NULL)
        {
            struct hashy *next = cur->next;

            free(cur);

            cur = next;
        }
        tab[i] = NULL;
    }
}
