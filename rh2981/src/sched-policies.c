#include <os-scheduling.h>

/*******************************
 * QUEUE MANAGEMENT PRIMITIVES *
 *******************************/

void printQueues(task tasks[], sched_data* schedData) {
    int i, j, taskIndex = 0;
    printf("Nb of queues %d\n", schedData->nbOfQueues);
    for (i = 0; i < schedData->nbOfQueues; i++) {
        j = 0;
        printf("Q%d => ", i);
        while (j < MAX_NB_OF_TASKS) {
            taskIndex = schedData->queues[i][j];
            if (taskIndex == -1) {
                j = MAX_NB_OF_TASKS;
            } else {
                printf("%s ", tasks[taskIndex].name);
                j++;
            }
        }
        printf("\n");
    }
}

void initQueues(int nbQ, sched_data* schedData) {
    int i, j;
    printf("Initializing %d job queue(s)\n", nbQ);
    schedData->nbOfQueues = nbQ;
    for (j = 0; j < nbQ; j++) {
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[j][i] = -1;
        }
    }
}

int enqueue(sched_data* schedData, int queueIndex, int taskIndex) {
    int end = 0;
    while ((end < MAX_NB_OF_TASKS) && (schedData->queues[queueIndex][end] != -1))
        end++;
    if (end < MAX_NB_OF_TASKS) {
        schedData->queues[queueIndex][end] = taskIndex;
        return 0;
    } else {
        // memory exhausted
        return -1;
    }
}

int dequeue(sched_data* schedData, int queueIndex) {
    int j;
    int taskIndex = schedData->queues[queueIndex][0];
    if (taskIndex != -1) {
        for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
            schedData->queues[queueIndex][j] = schedData->queues[queueIndex][j+1];
        }
        schedData->queues[queueIndex][MAX_NB_OF_TASKS - 1] = -1;
    }
    return taskIndex;
}

int dequeueIndex(sched_data* schedData, int queueIndex, int taskQueue) {
    // find the length of the queue
    int end = 0;
    int i, j,taskIndex;

    while ((end < MAX_NB_OF_TASKS) && (schedData->queues[queueIndex][end] != -1))
        end++;
    end --;

    for (i = 0; i<end; i++){
        taskIndex = schedData->queues[queueIndex][i];
        if (taskIndex == taskQueue){
            // find the task 
            break;
        }
    }

    for (j = i; j < end; j++) {
        //printf("++++++++++++++++++++++++++++++++++++\n");
        //printf("%d",schedData->queues[0][i]);
            schedData->queues[queueIndex][j] = schedData->queues[queueIndex][j+1];
        }
    schedData->queues[queueIndex][end] = -1;

    return taskQueue;
}


int head(sched_data* schedData) {
    printf("hhha");
    return schedData->queues[0][0];
}


int accesNextAvailable(sched_data* schedData, struct task tasks[])
{
    // find the head task in the queue which state is Ready
    int i = 0;
    int taskIndex;
    int taskstate;
    bool found = false;
    while ((i < MAX_NB_OF_TASKS) && (schedData->queues[0][i] != -1)){
        taskIndex = schedData->queues[0][i];
        taskstate = tasks[taskIndex].state;
        if (taskstate == RUNNING){
            // return the task that is running
            //tasks[taskIndex].state = READY;
            return schedData->queues[0][i];
        }
        if (taskstate == READY){
            found = true;
            break;
        }
        i++;
    }

    if(found == true){
        // printf("++++++++++++++++++++++++++++++++++++\n");
        // printf("%d",schedData->queues[0][i]);
        return schedData->queues[0][i];
    }else{
        // printf("NONONONONO");
        return -1;
        // not found
    }
}

/*******************************
 * TASKS MANAGEMENT PRIMITIVES *
 *******************************/

int admitNewTasks(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j;
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            tasks[i].stage1 = false; // this task has not wenthrough stage1
            tasks[i].stage2 = false; // this task has not wenthotugh stage2
            tasks[i].stage3 = false; // this task has not wenthotugh stage3
            tasks[i].cyclesInQuantum = 0;
            tasks[i].cyclesInBlocked = 0;
            tasks[i].cyclesInBlocked2 = 0;
            schedData->queues[0][j] = i;
            j++;
        }
    }
    return 1;
}

/***********************
 * INCREMENTING ALL TASK's waiting time *
 ***********************/

void incrementwaitingtime(task tasks[], int nbOfTasks){
    // at a clock frame, iterating all the blocked tasks, increment their waiting time by one
    int j;
    //iterating throught the queue to see if any task is in the blocked state
    for (j = 0; j<nbOfTasks; j++){
        if (tasks[j].state == BLOCKED){
            // if the task's staus if blocked
            if (tasks[j].stage1 == true){
                // printf("====================================================\n");
                if(tasks[j].stage2 == false){
                    // printf("===================Get Ready=============================\n");
                    if(tasks[j].cyclesInBlocked >= tasks[j].B1){
                        // the task has waited enough time, set ready
                        tasks[j].cyclesInBlocked = 0;
                        tasks[j].state = READY;
                        // eliminate the time the task spends in waiting
                    }
                    tasks[j].cyclesInBlocked ++;
                }else{
                    // stage2 = true
                    // printf("===================Get Ready2=============================\n");
                    if(tasks[j].cyclesInBlocked2 >= tasks[j].B2){
                        // the task has waited enough time, set ready
                        tasks[j].state = READY;
                    }
                    tasks[j].cyclesInBlocked2 ++;
                }
            }
            // increment waiting time
            
        }
    }
}

/***********************
 * SCHEDULING POLICIES *
 ***********************/

int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {

    int i;
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        initQueues(1, schedData);
    }
    
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task

    i = accesNextAvailable(schedData,tasks);
    
    if (i != -1) {
        if(tasks[i].stage1 == false){
                if (tasks[i].executionTime == tasks[i].R1){
                    tasks[i].state = BLOCKED;
                    tasks[i].stage1 = true;
                    //return accesNextAvailable(schedData,tasks);
                }
        }else{
            if (tasks[i].stage2 == false){
                if (tasks[i].executionTime == tasks[i].R2+tasks[i].R1){
                    tasks[i].state = BLOCKED;
                    // set its state to BLOCKED
                    tasks[i].stage2 = true;
                    //return accesNextAvailable(schedData,tasks);
                }
            }
        }
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime >= tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                // tasks[i].completionDate = currentTime;
                // dequeue corresponding task in the queue
                dequeueIndex(schedData, 0,i);
            } else {
                /* Reelect this task */
                if (tasks[i].executionTime+1 >= tasks[i].computationTime) {
                    tasks[i].state = TERMINATED;
                    dequeueIndex(schedData, 0,i);
                }

                // check if the task has reached its stage
                if(tasks[i].stage1 == false){
                    // if the task has not went through stage one
                    if (tasks[i].executionTime == tasks[i].R1){
                        // the task has run to its stage1 time
                        tasks[i].state = BLOCKED;
                        // set its state to BLOCKED
                        tasks[i].stage1 = true;
                        return accesNextAvailable(schedData,tasks);
                    }
                }else{
                    // the task has passed its stage one and haven't tho stage two
                    if(tasks[i].stage3 != true){
                        if (tasks[i].stage2 == false){
                            if (tasks[i].executionTime == tasks[i].R2+tasks[i].R1){
                                tasks[i].state = BLOCKED;
                                // set its state to BLOCKED
                                tasks[i].stage2 = true;
                                return accesNextAvailable(schedData,tasks);
                            }
                        }
                    }
                }
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    //i = head(schedData);

    i = accesNextAvailable(schedData,tasks);

    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        // if one task is running, shut down others task that might be running
        int j;
        for (j =0; j<nbOfTasks;j++){
            if ((j!=i)&&(tasks[j].state == RUNNING)){
                tasks[j].state = READY;
            }
        }

        if(tasks[i].stage1 == false){
                if (tasks[i].executionTime == tasks[i].R1){
                    tasks[i].state = BLOCKED;
                    tasks[i].stage1 = true;
                    return accesNextAvailable(schedData,tasks);
                }
        }else{
            if (tasks[i].stage2 == false){
                if (tasks[i].executionTime == tasks[i].R2+tasks[i].R1){
                    tasks[i].state = BLOCKED;
                    // set its state to BLOCKED
                    tasks[i].stage2 = true;
                    return accesNextAvailable(schedData,tasks);
                }
            }
        }
        return i;
    }
    
    // No task could be elected
    return -1;
}


int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i;
    int quantum = schedData->quantum;
    //printf("hellllo");
    if (currentTime == 0) {
        printf("RR> Initializing job queue\n");
        initQueues(1, schedData);
        printf("RR> Quantum duration is %d cycles",quantum);
    }

    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);

    // Is there a task running?
    //      => determine its remaining computation time
    //   Has that task finished its computations?
    //      => put it in terminated state
    //   else
    //      Is the task running exceeds the maximun allocated Quantum?
    //          => put it back to READY
    //          else 
    //              put it into the back of the queue

    i = accesNextAvailable(schedData,tasks);

    if (i!=-1){
        if(tasks[i].stage1 == false){
            if (tasks[i].executionTime == tasks[i].R1){
                tasks[i].state = BLOCKED;
                tasks[i].stage1 = true;
            }
        }else{
            if (tasks[i].stage2 == false){
                if (tasks[i].executionTime == tasks[i].R2+tasks[i].R1){
                    tasks[i].state = BLOCKED;
                    // set its state to BLOCKED
                    tasks[i].stage2 = true;
                }
            }
        }
        if (tasks[i].state == RUNNING) {
            if(tasks[i].stage1 == false){
                // if the task has not went through stage one
                if (tasks[i].executionTime == tasks[i].R1){
                    // the task has run to its stage1 time
                    tasks[i].state = BLOCKED;
                    // set its state to BLOCKED
                    tasks[i].stage1 = true;
                    //return accesNextAvailable(schedData,tasks);
                }
            }else{
                // the task has passed its stage one and haven't tho stage two
                if(tasks[i].stage3 != true){
                    if (tasks[i].stage2 == false){
                        if (tasks[i].executionTime == tasks[i].R2+tasks[i].R1){
                            tasks[i].state = BLOCKED;
                            // set its state to BLOCKED
                            tasks[i].stage2 = true;
                            //return accesNextAvailable(schedData,tasks);
                        }
                    }
                }
            }
            // if the Task has reached its required time, dequeue it
            if (tasks[i].executionTime == tasks[i].computationTime) {
                // printf("%d",tasks[i].cyclesInQuantum);
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                dequeue(schedData, 0);
            } 
            else {
                
                // else check wheter the task has reached its allocated quantum time
                if(tasks[i].cyclesInQuantum < quantum){
                    tasks[i].executionTime ++;
                    tasks[i].cyclesInQuantum++;
                    // check if the task has reached its stage
                    
                    return i;
                    // time taken in the quantum ++
                }
                else{
                    // else the task has reached its allocated time
                    printf("yes deque");
                    tasks[i].state = READY;
                    // dequeue it, push out the queue
                    dequeueIndex(schedData, 0, i);
                    // enqueue it, once again put it back
                    enqueue(schedData,0,i);
                    tasks[i].cyclesInQuantum = 0;
                     if(tasks[i].stage1 == false){
                        // if the task has not went through stage one
                        if (tasks[i].executionTime == tasks[i].R1){
                            // the task has run to its stage1 time
                            tasks[i].state = BLOCKED;
                            // set its state to BLOCKED
                            tasks[i].stage1 = true;
                            return accesNextAvailable(schedData,tasks);
                        }
                    }else{
                        // the task has passed its stage one and haven't tho stage two
                        if(tasks[i].stage3 != true){
                            if (tasks[i].stage2 == false){
                                if (tasks[i].executionTime == tasks[i].R2+tasks[i].R1){
                                    tasks[i].state = BLOCKED;
                                    // set its state to BLOCKED
                                    tasks[i].stage2 = true;
                                    return accesNextAvailable(schedData,tasks);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Now, elect the first task in the queue
    i = accesNextAvailable(schedData,tasks);

    if (i != -1){
        tasks[i].cyclesInQuantum = 0;
        tasks[i].executionTime ++;
        tasks[i].cyclesInQuantum ++;
        tasks[i].state = RUNNING;
        if(tasks[i].stage1 == false){
        // if the task has not went through stage one
                if (tasks[i].executionTime == tasks[i].R1){
                    // the task has run to its stage1 time
                    tasks[i].state = BLOCKED;
                    // set its state to BLOCKED
                    tasks[i].stage1 = true;
                    // return accesNextAvailable(schedData,tasks);
                    // quit this cycle
                    // chose next available
                }
            }else{
                // the task has passed its stage one and haven't tho stage two
                if(tasks[i].stage3 != true){
                    if (tasks[i].stage2 == false){
                        if (tasks[i].executionTime == tasks[i].R2+tasks[i].R1){
                            tasks[i].state = BLOCKED;
                            // set its state to BLOCKED
                            tasks[i].stage2 = true;
                            // return accesNextAvailable(schedData,tasks);
                        }
                    }
                }
            }

        // if one task is running, shut down others task that might be running
        int j;
        for (j =0; j<nbOfTasks;j++){
            if ((j!=i)&&(tasks[j].state == RUNNING)){
                tasks[j].state = READY;
                
            }
        }
        
        return i;
    }

    return -1;
}