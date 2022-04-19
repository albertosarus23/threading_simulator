/*********************************/
/* adapted sturture from professor Oliver Marin, NYUSH */
/* struture altered by Ruixiang Han to adapt Spring2022 OS Lab 2*/ 
/* algorithmn written and implemented by Ruixiang Han rh2981 , NYU */
/*********************************/
#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 512
#define MAX_NB_OF_TASKS 32
#define MAX_TASK_NAME_SIZE 32
#define MAX_NB_OF_QUEUES 8

#define UPCOMING 0
#define READY 1
#define RUNNING 2
#define BLOCKED 3
#define TERMINATED 4


typedef struct task {
    /* Initial elements */
    char name[MAX_TASK_NAME_SIZE]; //task name
    unsigned int computationTime; //task duration
    unsigned int arrivalDate; //date of insertion in the system
    /* Used by scheduler */
    unsigned int state;
    unsigned int executionTime; //nb of cycles on processor
    unsigned int cyclesInQuantum;
    unsigned int cyclesInBlocked;
    unsigned int cyclesInBlocked2;
    unsigned int completionDate;
    unsigned int R1;
    unsigned int B1;
    unsigned int R2;
    unsigned int B2;
    bool stage1;
    bool stage2;
    bool stage3;
} task;

typedef struct sched_data {
    int quantum;
    int nbOfQueues;
    int queues[MAX_NB_OF_QUEUES][MAX_NB_OF_TASKS];
} sched_data;

void printTasks(task tasks[], int nbOfTasks);

int accesNextAvailable(sched_data* schedData, struct task tasks[]);

void printQueues(task tasks[], sched_data* schedData);

int head(sched_data* schedData);

void incrementwaitingtime(task tasks[], int nbOfTasks);

int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime);

int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime);
