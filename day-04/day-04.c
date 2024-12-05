#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Count lines in file
int countLines(FILE *in)
{
    int count = 0;
    int cur;

    cur = fgetc(in);
    while (cur != '\n' && cur != EOF)
    {
        count++;
        cur = fgetc(in);
    }

    return count;
}

// Count occurences of str2 in str1
// Not in use: from previous attempt of matrix decomposition
int countSubstring(char *str1, char *str2)
{
    int count = 0;
    int str1Len = strlen(str1);
    int str2Len = strlen(str2);

    int i;
    for (i = 0; i <= str1Len-str2Len; i++)
    {
        int match = 1;
        
        int j;
        for (j = 0; j < str2Len; j++)
        {
            if (str1[i + j] != str2[j])
            {
                match = 0;
            }
        }

        if (match)
        {
            count++;
        }
    }

    return count;
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

    // Determine height and width
    int WIDTH = countLines(input);

    int len;
    fseek(input, 0, SEEK_END);
    len = ftell(input);
    rewind(input);

    int HEIGHT = (len + 1 - WIDTH)/WIDTH; // Final line missing LF

    // Read input, \n\0 -> \0
    char arr[HEIGHT][WIDTH+1];
    int i;
    for (i = 0; i < HEIGHT; i++)
    {
        char temp[WIDTH+2];
        if (fgets(temp, WIDTH+2, input) != NULL)
        {
            char *ptr = strchr(temp, '\n');
            if (ptr)
            {
                *ptr = '\0';
            }

            strncpy(arr[i], temp, WIDTH+1);
        }
    }

    // close
    fclose(input);

    int count = 0;
    int count2 = 0;
    for (i = 0; i < HEIGHT; i++)
    {
        int j;
        for (j = 0; j < WIDTH; j++)
        {
            // Part 1 cases
            if (arr[i][j] == 'X')
            {
                if (j+3 < WIDTH && arr[i][j+1] == 'M' && arr[i][j+2] == 'A' && arr[i][j+3] == 'S') // X > M > A > S
                {
                    count++;
                }
                if (i+3 < HEIGHT && arr[i+1][j] == 'M' && arr[i+2][j] == 'A' && arr[i+3][j] == 'S') // X v M v A v S
                {
                    count++;
                }
                if (i+3 < HEIGHT && j+3 < WIDTH && arr[i+1][j+1] == 'M' && arr[i+2][j+2] == 'A' && arr[i+3][j+3] == 'S') // X >v M >v A >v S
                {
                    count++;
                }
                if (i+3 < HEIGHT && j-3 >= 0 && arr[i+1][j-1] == 'M' && arr[i+2][j-2] == 'A' && arr[i+3][j-3] == 'S') // X <v M <v A <v S
                {
                    count++;
                }
            }
            else if (arr[i][j] == 'S')
            {
                if (j+3 < WIDTH && arr[i][j+1] == 'A' && arr[i][j+2] == 'M' && arr[i][j+3] == 'X') // S > A > M > X
                {
                    count++;
                }
                if (i+3 < HEIGHT && arr[i+1][j] == 'A' && arr[i+2][j] == 'M' && arr[i+3][j] == 'X') // S v A v M v X
                {
                    count++;
                }
                if (i+3 < HEIGHT && j+3 < WIDTH && arr[i+1][j+1] == 'A' && arr[i+2][j+2] == 'M' && arr[i+3][j+3] == 'X') // S >v A >v M >v X
                {
                    count++;
                }
                if (i+3 < HEIGHT && j-3 >= 0 && arr[i+1][j-1] == 'A' && arr[i+2][j-2] == 'M' && arr[i+3][j-3] == 'X') // S <v A <v M <v X
                {
                    count++;
                }
            }

            // Part 2
            if (arr[i][j] == 'A')
            {
                if (i >= 1 && j >= 1 && i+1 < HEIGHT && j+1 < WIDTH
                && ((arr[i-1][j-1] == 'M' && arr[i+1][j+1] == 'S')
                  ||(arr[i-1][j-1] == 'S' && arr[i+1][j+1] == 'M'))
                && ((arr[i+1][j-1] == 'M' && arr[i-1][j+1] == 'S')
                  ||(arr[i+1][j-1] == 'S' && arr[i-1][j+1] == 'M')))
                {
                    count2++;
                }
            }
        }
    }

    printf("Part 1: %d\n", count);
    printf("Part 2: %d\n", count2);

    return 0;
}