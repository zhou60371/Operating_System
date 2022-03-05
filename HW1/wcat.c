#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    if (argc == 1)
    {
        exit(0);
    }
    for (int i = 1; i < argc; i++)
    {
        if ((fp = fopen(*++argv, "r")) == NULL)
        {
            puts("wcat: cannot open file");
            exit(1);
        }
        else
        {
            char buffer[4096];
            while (fgets(buffer, 4096, fp) != NULL)
            {
                char *ptr = strchr(buffer, '\n');
                if (ptr)
                {
                    //if new line found replace with null character
                    *ptr = '\0';
                }
                puts(buffer);
            }
            fclose(fp);
        }
    }
    exit(0);
}
