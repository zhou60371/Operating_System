// Author: Zhou Zihao
// OS project 3: time schedual simulation

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct job
{
    /* data */
    int id;
    int length;
    int arrive;
    struct job *next;
    struct job *pre;
};

int analysis;
int mode;
FILE *fp;
struct job *head;
int count = 0;
int timeslice;

int initial(char *[]);
int run_FIFO();
int run_SJF();
int run_RR();
int analysis_FIFO(char **argv);
int analysis_SJF(char **argv);
int analysis_RR(char **argv);

int main(int argc, char *argv[])
{
    // initial parameters and create job list
    initial(argv);
    // type of time schedual policy
    if (strcmp(argv[2], "FIFO") == 0)
    {
        run_FIFO();
        if (analysis == 1)
        {
            analysis_FIFO(argv);
        }
    }
    else if (strcmp(argv[2], "SJF") == 0)
    {
        run_SJF();
        if (analysis == 1)
        {
            analysis_SJF(argv);
        }
    }
    else if (strcmp(argv[2], "RR") == 0)
    {
        run_RR();
        if (analysis == 1)
        {
            analysis_RR(argv);
        }
    }
    return 0;
}

int initial(char **parameters)
{
    if (strcmp(parameters[1], "0") == 0)
    {
        analysis = 0;
    }
    else if (strcmp(parameters[1], "1") == 0)
    {
        analysis = 1;
    }
    timeslice = atoi(parameters[4]);
    int id = 0;
    char str[20];
    char *temp;

    struct job *cur;
    char *filename = parameters[3];
    fp = fopen(filename, "r");
    while (fgets(str, 19, fp))
    {
        if (str[strlen(str) - 1] == '\n')
        {
            str[strlen(str) - 1] = '\0';
        }
        temp = strtok(str, ",");
        if (temp != NULL)
        {
            if (id == 0)
            {
                struct job *nextJob = malloc(sizeof(struct job));
                head = nextJob;
                head->id = id;
                head->arrive = atoi(temp);
                temp = strtok(NULL, ",");
                head->length = atoi(temp);
                head->next = NULL;
                head->pre = NULL;
                cur = head;
                id++;
            }
            else
            {
                struct job *nextJob = malloc(sizeof(struct job));
                nextJob->id = id;
                nextJob->arrive = atoi(temp);
                temp = strtok(NULL, ",");
                nextJob->length = atoi(temp);
                nextJob->next = NULL;
                nextJob->pre = cur;
                cur->next = nextJob;
                cur = nextJob;
                id++;
            }
        }
    }
    count = id;
    return 0;
}

int run_FIFO()
{
    printf("Execution trace with FIFO:\n");
    struct job *point = head;
    int t = 0;
    while (point->next != NULL)
    {
        printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", t, point->id, point->arrive, point->length);
        t = t + point->length;
        point = point->next;
    }
    printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", t, point->id, point->arrive, point->length);
    printf("End of execution with FIFO.\n");
    return 0;
}

int run_SJF()
{
    int t = 0;
    int remain = count;
    struct job *point;
    struct job *cur;
    printf("Execution trace with SJF:\n");
    while (remain > 0)
    {
        point = head;
        cur = point;
        // printf("head: %d\n", head->id);
        // printf("head: %p\n", head->pre);
        int cur_length = cur->length;
        for (int i = 0; i < remain; i++)
        {
            // printf("%d\n", point->id);
            if ((point->arrive) > t)
            {
                break;
            }
            else
            {
                if ((point->length) < cur_length)
                {
                    cur = point;
                    cur_length = cur->length;
                }
            }
            point = point->next;
        }
        if (cur->arrive > t)
        {
            t = cur->arrive;
        }
        printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", t, cur->id, cur->arrive, cur->length);
        t = t + cur->length;
        remain--;
        if (remain > 0)
        {
            if (cur->next == NULL)
            {
                cur->pre->next = NULL;
                free(cur);
            }
            else if (cur->pre == NULL)
            {
                head = cur->next;
                head->pre = NULL;
                free(cur);
            }
            else
            {
                cur->pre->next = cur->next;
                cur->next->pre = cur->pre;
                free(cur);
            }
        }
    }
    printf("End of execution with SJF.\n");
    return 0;
}

int run_RR()
{
    struct job *cur = head;
    int remain = count;
    int t = 0;
    int ran_for = 0;
    printf("Execution trace with RR:\n");
    while (remain > 0)
    {
        if (cur->length <= timeslice)
        {
            ran_for = cur->length;
            t = (t < cur->arrive) ? (cur->arrive) : t;
            printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", t, cur->id, cur->arrive, ran_for);
            t = t + ran_for;
            remain--;
            // free this task
            struct job *temp = cur;
            if (remain > 0)
            {
                if (cur->next == NULL)
                {
                    cur = head;
                    temp->pre->next = NULL;
                    free(temp);
                }
                else if (cur->pre == NULL)
                {
                    head = cur->next;
                    head->pre = NULL;
                    cur = cur->next;
                    free(temp);
                }
                else
                {
                    cur->pre->next = cur->next;
                    cur->next->pre = cur->pre;
                    if (cur->next->arrive > t)
                    {
                        cur = head;
                    }
                    else
                    {
                        cur = cur->next;
                    }

                    free(temp);
                }
            }
        }
        else
        {
            ran_for = timeslice;
            t = (t < cur->arrive) ? (cur->arrive) : t;
            printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", t, cur->id, cur->arrive, ran_for);
            t = t + timeslice;
            cur->length = cur->length - timeslice;
            if (remain > 1)
            {
                if (cur->next == NULL)
                {
                    cur = head;
                }
                else if (cur->pre == NULL)
                {
                    if (cur->next->arrive > t)
                    {
                        cur = head;
                    }
                    else
                    {
                        cur = cur->next;
                    }
                }
                else
                {
                    if (cur->next->arrive > t)
                    {
                        cur = head;
                    }
                    else
                    {
                        cur = cur->next;
                    }
                }
            }
        }
    }
    printf("End of execution with RR.\n");
    return 0;
}

int analysis_FIFO(char **argv)
{
    double sum_response = 0;
    double sum_turnaround = 0;
    double sum_wait = 0;
    initial(argv);
    printf("Begin analyzing FIFO:\n");
    struct job *point = head;
    int t = 0;
    while (point->next != NULL)
    {
        sum_response += (t - point->arrive);
        sum_turnaround += (t - point->arrive + point->length);
        sum_wait += (t - point->arrive);
        printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n", point->id, (t - point->arrive), (t - point->arrive + point->length), (t - point->arrive));
        t = t + point->length;
        point = point->next;
    }
    sum_response += (t - point->arrive);
    sum_turnaround += (t - point->arrive + point->length);
    sum_wait += (t - point->arrive);

    printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n", point->id, (t - point->arrive), (t - point->arrive + point->length), (t - point->arrive));
    printf("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n", (sum_response / count), sum_turnaround / count, sum_wait / count);
    printf("End analyzing FIFO.\n");
    return 0;
}

int analysis_SJF(char **argv)
{
    struct info
    {
        double response;
        double turnaround;
        double wait;
    };
    struct info *infos[count];
    initial(argv);
    int t = 0;
    int remain = count;
    struct job *point;
    struct job *cur;
    double sum_response = 0;
    double sum_turnaround = 0;
    double sum_wait = 0;

    printf("Begin analyzing SJF:\n");
    while (remain > 0)
    {
        point = head;
        cur = point;
        int cur_length = cur->length;
        for (int i = 0; i < remain; i++)
        {
            if ((point->arrive) > t)
            {
                break;
            }
            else
            {
                if ((point->length) < cur_length)
                {
                    cur = point;
                    cur_length = cur->length;
                }
            }
            point = point->next;
        }
        if (cur->arrive > t)
        {
            t = cur->arrive;
        }
        sum_response += (t - cur->arrive);
        sum_turnaround += (t - cur->arrive + cur->length);
        sum_wait += (t - cur->arrive);
        struct info *temp = malloc(sizeof(struct info));
        temp->response = t - cur->arrive;
        temp->turnaround = t - cur->arrive + cur->length;
        temp->wait = t - cur->arrive;
        infos[cur->id] = temp;
        // printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n", cur->id, (t - cur->arrive), (t - cur->arrive + cur->length), (t - cur->arrive));
        t = t + cur->length;
        remain--;
        if (remain > 0)
        {
            if (cur->next == NULL)
            {
                cur->pre->next = NULL;
                free(cur);
            }
            else if (cur->pre == NULL)
            {
                head = cur->next;
                head->pre = NULL;
                free(cur);
            }
            else
            {
                cur->pre->next = cur->next;
                cur->next->pre = cur->pre;
                free(cur);
            }
        }
    }
    for (int i = 0; i < count; i++)
    {
        printf("Job %d -- Response time: %.0f  Turnaround: %.0f  Wait: %.0f\n", i, infos[i]->response, infos[i]->turnaround, infos[i]->wait);
    }
    printf("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n", sum_response / count, sum_turnaround / count, sum_wait / count);
    printf("End analyzing SJF.\n");
    return 0;
}

int analysis_RR(char **argv)
{
    double sum_response = 0;
    double sum_turnaround = 0;
    double sum_wait = 0;
    struct info
    {
        double turnaround;
        double wait;
        double last_exect;
    };
    struct info *infos[count];
    int response[count];
    for (int i = 0; i < count; i++)
    {
        response[i] = -1;
    }
    initial(argv);
    struct job *cur = head;
    int remain = count;
    int t = 0;
    int ran_for = 0;
    printf("Begin analyzing RR:\n");
    while (remain > 0)
    {
        if (cur->length <= timeslice)
        {
            ran_for = cur->length;
            t = (t < cur->arrive) ? (cur->arrive) : t;
            if (response[cur->id] == -1)
            {
                struct info *temp_info = malloc(sizeof(struct info));
                response[cur->id] = t - cur->arrive;
                sum_response += (t - cur->arrive);
                temp_info->turnaround = t + ran_for - cur->arrive;
                temp_info->wait = t - cur->arrive;
                sum_wait += (t - cur->arrive);
                t = t + ran_for;
                temp_info->last_exect = t;
                infos[cur->id] = temp_info;
            }
            else
            {
                infos[cur->id]->turnaround = t + ran_for - cur->arrive;
                infos[cur->id]->wait += (t - infos[cur->id]->last_exect);
                sum_wait += (t - infos[cur->id]->last_exect);
                t = t + ran_for;
                infos[cur->id]->last_exect = t;
            }

            // printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", t, cur->id, cur->arrive, ran_for);

            remain--;
            // free this task
            struct job *temp = cur;
            if (remain > 0)
            {
                if (cur->next == NULL)
                {
                    cur = head;
                    temp->pre->next = NULL;
                    free(temp);
                }
                else if (cur->pre == NULL)
                {
                    head = cur->next;
                    head->pre = NULL;
                    cur = cur->next;
                    free(temp);
                }
                else
                {
                    cur->pre->next = cur->next;
                    cur->next->pre = cur->pre;
                    if (cur->next->arrive > t)
                    {
                        cur = head;
                    }
                    else
                    {
                        cur = cur->next;
                    }

                    free(temp);
                }
            }
        }
        else
        {
            ran_for = timeslice;
            t = (t < cur->arrive) ? (cur->arrive) : t;

            if (response[cur->id] == -1)
            {
                struct info *temp_info = malloc(sizeof(struct info));
                response[cur->id] = t - cur->arrive;
                sum_response += (t - cur->arrive);
                temp_info->turnaround = t + ran_for - cur->arrive;
                temp_info->wait = t - cur->arrive;
                sum_wait += (t - cur->arrive);
                t = t + ran_for;
                temp_info->last_exect = t;
                infos[cur->id] = temp_info;
            }
            else
            {
                infos[cur->id]->turnaround = t + ran_for - cur->arrive;
                infos[cur->id]->wait += (t - infos[cur->id]->last_exect);
                sum_wait += (t - infos[cur->id]->last_exect);
                t = t + ran_for;
                infos[cur->id]->last_exect = t;
            }
            // printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", t, cur->id, cur->arrive, ran_for);
            cur->length = cur->length - timeslice;
            if (remain > 1)
            {
                if (cur->next == NULL)
                {
                    cur = head;
                }
                else if (cur->pre == NULL)
                {
                    if (cur->next->arrive > t)
                    {
                        cur = head;
                    }
                    else
                    {
                        cur = cur->next;
                    }
                }
                else
                {
                    if (cur->next->arrive > t)
                    {
                        cur = head;
                    }
                    else
                    {
                        cur = cur->next;
                    }
                }
            }
        }
    }
    for (int i = 0; i < count; i++)
    {
        sum_turnaround = sum_turnaround + infos[i]->turnaround;
        printf("Job %d -- Response time: %d  Turnaround: %.0f  Wait: %.0f\n", i, response[i], infos[i]->turnaround, infos[i]->wait);
    }
    printf("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n", sum_response / count, sum_turnaround / count, sum_wait / count);
    printf("End analyzing RR.\n");
    return 0;
}