#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        puts("wzip: file1 [file2 ...]");
        exit(1);
    }
    else
    {
        char c, temp;
        FILE *fp;
        int count;
        for (int i = 1; i < argc; i++)
        {
            fp = fopen(argv[i], "r");
            if (i == 1)
            {
                // if ((c = getc(fp)) == EOF)
                if (fread(&c, 1, 1, fp) != 1)
                {
                    exit(0);
                }
                temp = c;
                count = 1;
            }
            // while ((c = getc(fp)) != EOF)
            while (fread(&c, 1, 1, fp) == 1)
            {
                if (c != temp)
                {
                    fwrite(&count, 4, 1, stdout);
                    fwrite(&temp, 1, 1, stdout);
                    temp = c;
                    count = 1;
                }
                else
                {
                    count++;
                }
            }
            fclose(fp);
        }
        fwrite(&count, 4, 1, stdout);
        fwrite(&temp, 1, 1, stdout);
        exit(0);
    }
}