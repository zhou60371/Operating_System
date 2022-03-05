// Place code here.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define TRUE 1

char *strings1[300];
char *strings2[300];
int cgroup = 1;
int number1;
int number2;
int mode = 0;
int hasSpace = 0;
char *commands[10];
int qin = 0;
int qout = 0;
int size = 0;
int backgroundProcess[255];
char *backgroundCommand[255];
char *commandNow;
int bcpt = 1;
int type_prompt();
int read_command(char **strings, int number);

int main(int argc, char *argv[])
{
    int type_prompt();
    int read_command(char **strings, int number);
    while (TRUE)
    {
        // type prompt
        type_prompt();
        // read_command();
        if (mode == 0 || mode == 3 || mode == 4 || mode == 5 || mode == 6)
        {
            read_command(strings1, number1);
        }
        else if (mode == 1)
        {
            int result = read_command(strings1, number1);
            if (result == 0)
            {
                read_command(strings2, number2);
            }
        }
        else if (mode == 2)
        {
            int result = read_command(strings1, number1);
            if (result == 1)
            {
                read_command(strings2, number2);
            }
        }
    }
    exit(0);
}

int type_prompt()
{
    char buf[320];
    char *dirn;
    char str[300];
    char *temp;
    number1 = 0;
    number2 = 0;
    cgroup = 1;
    mode = 0;

    // check background list
    for (int i = 0; i < 255; i++)
    {

        if ((backgroundProcess[i] != 0) && (waitpid(backgroundProcess[i], NULL, WNOHANG) != 0))
        {
            printf("[%d] Done: %s \n", i + 1, backgroundCommand[i]);
            backgroundProcess[i] = 0;
        }
    }

    // print working dirctory
    getcwd(buf, sizeof(buf));
    dirn = basename(buf);
    printf("%s$ ", dirn);
    fflush(stdout);
    fgets(str, 300, stdin);
    str[strlen(str) - 1] = '\0';
    if (str[strlen(str) - 1] == ' ')
    {
        hasSpace = 1;
    }

    // store in commans
    size < 10 ? size++ : (qout = (qout + 1) % 10);
    commands[qin] = strdup(str);
    qin = (qin + 1) % 10;
    commandNow = strdup(str);
    commandNow[strlen(commandNow) - 2] = '\0';

    if (!isatty(fileno(stdin)))
    {
        printf("%s\n", str);
        fflush(stdout);
    }

    temp = strtok(str, " ");
    while (temp != NULL && number1 < 299)
    {
        if (strcmp(temp, "&&") == 0)
        {
            cgroup = 2;
            temp = strtok(NULL, " ");
            mode = 1;
            continue;
        }
        if (strcmp(temp, "||") == 0)
        {
            cgroup = 2;
            temp = strtok(NULL, " ");
            mode = 2;
            continue;
        }
        if (strcmp(temp, ">") == 0)
        {
            cgroup = 2;
            temp = strtok(NULL, " ");
            mode = 3;
            continue;
        }
        if (strcmp(temp, ">>") == 0)
        {
            cgroup = 2;
            temp = strtok(NULL, " ");
            mode = 4;
            continue;
        }
        if (strcmp(temp, "|") == 0)
        {
            cgroup = 2;
            temp = strtok(NULL, " ");
            mode = 5;
            continue;
        }
        if (strcmp(temp, "&") == 0)
        {
            temp = strtok(NULL, " ");
            mode = 6;
            continue;
        }
        if (cgroup == 1)
        {
            strings1[number1] = strdup(temp);
            number1++;
        }
        else if (cgroup == 2)
        {
            strings2[number2] = strdup(temp);
            number2++;
        }
        temp = strtok(NULL, " ");
    }

    if (hasSpace == 1 && cgroup == 2)
    {
        strcat(strings2[number2 - 1], " ");
    }
    return 0;
}

int read_command(char **strings, int number)
{
    if (strings[0] != NULL)
    {
        if (strcmp(strings[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(strings[0], "echo") == 0 && mode != 3 && mode != 4 && mode != 5)
        {
            for (int i = 1; i < number; i++)
            {
                if (i == number - 1)
                {
                    printf("%s", strings[i]);
                }
                else
                {
                    printf("%s ", strings[i]);
                }
            }
            printf("\n");
        }
        else if (strcmp(strings[0], "cd") == 0)
        {
            if (number == 1)
            {
                char *home;
                home = getenv("HOME");
                chdir(home);
            }
            else if (number > 2)
            {
                printf("wshell: cd: too many arguments\n");
            }
            else
            {
                int erro;
                erro = chdir(strings[1]);
                if (erro != 0)
                {
                    printf("wshell: no such directory: %s\n", strings[1]);
                }
            }
        }
        else if (strcmp(strings[0], "pwd") == 0)
        {
            char buf[320];
            getcwd(buf, sizeof(buf));
            printf("%s\n", buf);
        }
        else if (strcmp(strings[0], "history") == 0)
        {
            for (int i = 0; i < size; i++)
            {
                printf("%s\n", commands[(qout + i) % 10]);
            }
        }
        else if (strcmp(strings[0], "jobs") == 0)
        {
            for (int i = 0; i < 255; i++)
            {
                if ((backgroundProcess[i] != 0) && (waitpid(backgroundProcess[i], NULL, WNOHANG) == 0))
                {
                    printf("%d: %s \n", i + 1, backgroundCommand[i]);
                }
            }
        }
        else if (strcmp(strings[0], "kill") == 0)
        {
        	if (backgroundProcess[atoi(strings[1]) - 1] == 0)
            	{
                	printf("wshell: no such background job: %d\n", atoi(strings[1]));
                	return 0;
            	}
            	if ((kill(backgroundProcess[atoi(strings[1]) - 1], SIGKILL) == -1))
            	{
                	printf("wshell: no such background job: %d\n", atoi(strings[1]));
            	}
            	else
            	{
                	backgroundProcess[atoi(strings[1]) - 1] = 0;
            	}
        }
        else
        {
            if (mode == 5)
            {
                int pipefd[2];
                int child_A, child_B;
                int status;
                pipe(pipefd);
                child_A = fork();
                if (child_A == 0)
                {
                    close(pipefd[0]);
                    dup2(pipefd[1], 1);
                    close(pipefd[1]);
                    strings[number] = NULL;
                    int success = execvp(strings[0], strings);
                    if (success == -1)
                    {
                        printf("wshell: could not execute command: %s\n", strings[0]);
                        exit(1);
                    }
                }
                else
                {
                    waitpid(child_A, &status, 0);
                    if (status != 0)
                    {
                        return 1;
                    }
                    child_B = fork();
                    if (child_B == 0)
                    {
                        close(pipefd[1]);
                        dup2(pipefd[0], 0);
                        close(pipefd[0]);
                        strings2[number2] = NULL;
                        int success = execvp(strings2[0], strings2);
                        if (success == -1)
                        {
                            printf("wshell: could not execute command: %s\n", strings2[0]);
                            exit(1);
                        }
                    }
                    else
                    {
                        close(pipefd[0]);
                        close(pipefd[1]);
                        waitpid(child_B, &status, 0);
                        return (status == 0) ? 0 : 1;
                    }
                }
            }
            else if (mode == 6)
            {
                int status;
                int pid = fork();
                if (pid == 0)
                {
                    strings[number1] = NULL;
                    int success = execvp(strings[0], strings);
                    if (success == -1)
                    {
                        printf("wshell: could not execute command: %s\n", strings[0]);
                        exit(1);
                    }
                }
                else
                {
                    if (waitpid(pid, &status, WNOHANG) == 0)
                    {
                        printf("[%d]\n", bcpt);
                        backgroundProcess[bcpt - 1] = pid;
                        backgroundCommand[bcpt - 1] = commandNow;
                        if (bcpt < 255)
                        {
                            bcpt++;
                        }
                        else
                        {
                            bcpt = 1;
                        }
                    }
                    return (status == 0) ? 0 : 1;
                }
            }
            else
            {
                int status;
                int pid = fork();
                if (pid == 0)
                {
                    if (mode == 3)
                    {
                        fclose(stdout);
                        freopen(strings2[0], "w+", stdout);
                        // FILE *fq = freopen("/Users/zihao/Documents/Cources/OS/Project2/assigned-project-processes/kkk.text", "w+", stdout);
                    }
                    if (mode == 4)
                    {
                        fclose(stdout);
                        freopen(strings2[0], "a", stdout);
                    }
                    strings[number1] = NULL;
                    int success = execvp(strings[0], strings);
                    if (success == -1)
                    {
                        printf("wshell: could not execute command: %s\n", strings[0]);
                        exit(1);
                    }
                }
                else
                {
                    waitpid(pid, &status, 0);
                    // printf("status: %d\n", status);
                    return (status == 0) ? 0 : 1;
                }
            }
        }
    }
    return 0;
}
