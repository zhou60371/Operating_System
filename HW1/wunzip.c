#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        puts("wunzip: file1 [file2 ...]");
        exit(1);
    }
    else
    {
        FILE *fp;

        int loop;
        char c;
        for (int i = 1; i < argc; i++)
        {
            fp = fopen(argv[i], "r");
            while (1)
            {
                if (fread(&loop, 4, 1, fp) != 1)
                {
                    break;
                }
                if (fread(&c, 1, 1, fp) != 1)
                {
                    break;
                }
                // printf("c is : %c\n", c);
                for (int i = 0; i < loop; i++)
                {
                    printf("%c", c);
                }
            }
            fclose(fp);
        }
        exit(0);
    }
}
