#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        puts("wgrep: searchterm [file ...]");
        exit(1);
    }
    else if (argc == 2)
    {
        char *line = NULL;
        size_t n = 0;
        char *pattern = *++argv;
        while (getline(&line, &n, stdin) != -1)
        {
            if (strstr(line, pattern) != NULL)
            {
                char *ptr = strchr(line, '\n');
                if (ptr)
                {
                    //if new line found replace with null character
                    *ptr = '\0';
                }
                puts(line);
            }
        }
        free(line);
        exit(0);
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            FILE *fp;
            if ((fp = fopen(argv[i], "r")) == NULL)
            {
                puts("wgrep: cannot open file");
                exit(1);
            }
            else
            {
                char *line = NULL;
                size_t n = 0;
                char *pattern = argv[1];
                while (getline(&line, &n, fp) != -1)
                {
                    if (strstr(line, pattern) != NULL)
                    {
                        char *ptr = strchr(line, '\n');
                        if (ptr)
                        {
                            *ptr = '\0';
                        }
                        puts(line);
                    }
                }
                free(line);
            }
            fclose(fp);
        }
        exit(0);
    }
}