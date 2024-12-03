#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // file length
    int len;
    fseek(input, 0, SEEK_END);
    len = ftell(input);
    rewind(input);

    // read file contents
    char *buf = 0;
    buf = malloc(len * sizeof(char));
    fread(buf, 1, len, input);

    // close
    fclose(input);

    // Part 1
    int sum = 0;
    char *ptr = buf;
    
    // alternatively: (ptr = strstr(ptr, "mul(")) != NULL
    while (*ptr)
    {
        // Scan until "mul(" seen
        if (strncmp(ptr, "mul(", 4) != 0) {
            ptr++;
            continue;
        }

        ptr += 4;

        // Scan until the next closing bracket
        char *close = strchr(ptr, ')');
        if (close && close > ptr)
        {
            // Check that all characters between the brackets are \d or ,
            int uncor = 1;
            char *chk = ptr;
            for (chk = ptr; chk < close; chk++)
            {
                if (!isdigit(*chk) && *chk != ',')
                {
                    uncor = 0;
                    break;
                }
            }

            // Invalid characters spotted, skip this one
            if (!uncor)
            {
                continue;
            }

            // Extract characters between brackets into string
            char ps[128] = {0};
            strncpy(ps, ptr, close-ptr);
            ps[close-ptr] = '\0';

            // Extract numbers from string.
            // If %c matches, we have another comma, so this is invalid
            int p1;
            int p2;
            if (sscanf(ps, "%d,%d%c", &p1, &p2) == 2)
            {
                // Add multiplication to sum
                sum += (p1 * p2);
            }
        }

        ptr = close+1;
    }

    printf("PART 1: %d\n", sum);

    // Part 2
    ptr = buf;
    sum = 0;
    int doing = 1;
    while (*ptr)
    {
        // Scan until "mul(", "do()", or "don't()" seen
        if (strncmp(ptr, "mul(", 4) == 0) {
            ptr += 4;

            // Scan until the next closing bracket
            char *close = strchr(ptr, ')');
            if (close && close > ptr)
            {
                // Check that all characters between the brackets are \d or ,
                int uncor = 1;
                char *chk = ptr;
                for (chk = ptr; chk < close; chk++)
                {
                    if (!isdigit(*chk) && *chk != ',')
                    {
                        uncor = 0;
                        break;
                    }
                }

                // Invalid characters spotted, skip this one
                if (!uncor)
                {
                    continue;
                }

                // Extract characters between brackets into string
                char ps[128] = {0};
                strncpy(ps, ptr, close-ptr);
                ps[close-ptr] = '\0';

                // Extract numbers from string.
                // If %c matches, we have another comma, so this is invalid
                int p1;
                int p2;
                if (doing && sscanf(ps, "%d,%d%c", &p1, &p2) == 2)
                {
                    // Add multiplication to sum
                    // alternatively: sum += (p1 * p2 * doing);
                    sum += (p1 * p2);
                }
                ptr = close+1;
            }
        }
        else if (strncmp(ptr, "do()", 4) == 0) 
        {
            ptr += 4;
            doing = 1;
        }
        else if (strncmp(ptr, "don't()", 7) == 0)
        {
            ptr += 7;
            doing = 0;
        }
        else
        {
            // Keep scanning
            ptr++;
        }
    }

    printf("PART 2: %d\n", sum);

    // free
    free(buf);
}
