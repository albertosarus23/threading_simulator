/*********************************/
/* adapted sturture from professor Oliver Marin, NYUSH */
/* struture altered by Ruixiang Han to adapt Spring2022 OS Lab 2*/ 
/* algorithmn written and implemented by Ruixiang Han rh2981 , NYU */
/* ver2*/
/*********************************/

#include <os-scheduling.h>


char * states[] = {"upcoming", "ready   ", "running", "blocked ", "terminated"};


char * getStateString(int state) {
    return states[state];
}

/* Returns the number of tasks that still have to be run, */
/* that is, all tasks which still have computations to perform */
int hasTasksToSchedule(task tasks[], int nbOfTasks) {
    int total = 0;
    int i;
    
    for(i=0; i<nbOfTasks; i++) {
        if (tasks[i].state != TERMINATED) {
            total ++;
        }
    }
    return total;
}


void printTasks(task tasks[], int nbOfTasks) {
    int i;
    
    for(i=0; i<nbOfTasks; i++) {
        printf("Task: %s \t arrivalDate:%d    \t state:%s \t computations:%d/%d \t  cycleinblock:%d \n",
               tasks[i].name, tasks[i].arrivalDate, getStateString(tasks[i].state),
               tasks[i].executionTime, tasks[i].computationTime, tasks[i].cyclesInBlocked);
    }
}

int main(int argc, char *argv[]){
    struct task tasks[MAX_NB_OF_TASKS];
    sched_data *schedData = (sched_data*)malloc(sizeof(sched_data));
    int taskNum = 0;
    int time = 0;
    int taskIndex;

    int policy;
    
    // read input from the user 
    taskNum = atoi(argv[1]);
    policy = atoi(argv[2]);
    // schedData->quantum = atoi(argv[3]);
    // printf("%d%d%d\n",taskNum,policy,schedData->quantum);

    if (policy == 1){
        printf("Scheduling policy is %s\n", "FCFS");
        for (int i = 0; i < taskNum; i++)
        {
            
            tasks[i].name[0]='T';
            char charValue= i+'0';
            tasks[i].name[1]=charValue; //name
            tasks[i].name[2]='\0'; //name
            tasks[i].arrivalDate = 0;
            tasks[i].R1=atoi(argv[3+i*4]);
            printf("R1 is %d\n",tasks[i].R1);
            tasks[i].B1=atoi(argv[4+i*4]);
            tasks[i].R2=atoi(argv[5+i*4]);
            tasks[i].B2=atoi(argv[6+i*4]);
            tasks[i].executionTime = 0;
            tasks[i].computationTime = tasks[i].R1 + tasks[i].R2 + 1; // time needed for a task is R1+R2+1
            tasks[i].state = UPCOMING;
        }
    }
    else{
        // RR
        printf("Scheduling policy is %s\n", "RR");
        // adjusting policy input for RR
        schedData->quantum = atoi(argv[3]);
        for (int i = 0; i < taskNum; i++)
        { 
            tasks[i].name[0]='T';
            char charValue= i+'0';
            tasks[i].name[1]=charValue; //name
            tasks[i].name[2]='\0'; //name
            tasks[i].arrivalDate = 0;
            tasks[i].R1=atoi(argv[4+i*4]);
            tasks[i].B1=atoi(argv[5+i*4]);
            tasks[i].R2=atoi(argv[6+i*4]);
            printf("R1 is %d\n",tasks[i].R1);
            tasks[i].B2=atoi(argv[7+i*4]);
            tasks[i].executionTime = 0;
            tasks[i].computationTime = tasks[i].R1 + tasks[i].R2 + 1; // time needed for a task is R1+R2+1
            tasks[i].state = UPCOMING;
        }
    }
    /**** Schedule the set of tasks ****/
    printf("Scheduling the set of tasks\n");
    
    if (policy == 1){
        while(hasTasksToSchedule(tasks, taskNum) > 0) {
        // if there are still tasks to do
            // incrementing Blocked job's waiting time
            incrementwaitingtime(tasks,taskNum);
            printTasks(tasks, taskNum);
            taskIndex = FCFS(tasks, taskNum, schedData, time);
            if (taskIndex >= 0) {
                printf("\nTime %d: %s\n", time+1,  tasks[taskIndex].name);
            } else {
                printf("\nTime %d:  no task to schedule\n", time+1);
            }
            time ++;
        }       
    }
    else{
        while(hasTasksToSchedule(tasks, taskNum) > 0) {
        // if there are still tasks to do
        incrementwaitingtime(tasks,taskNum);
        printTasks(tasks, taskNum);
        taskIndex = RR(tasks, taskNum, schedData, time);
        if (taskIndex >= 0) {
            printf("\nTime %d: %s\n", time+1,  tasks[taskIndex].name);
        } else {
            printf("\nTime %d: no task to schedule\n", time+1);
        }
        time ++;
        }   
    }
    

    time -- ;
    printf("\n\nAll done after %d units of time\n", time);
    return 0;
}

