#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <bits/sigthread.h>
#include <signal.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define ROBOT_TOTAL 5
#define SIMULATOR_TOTAL 3

#define SIMULATION_TIME_SEC 1
#define TOTAL_SIMULATION_TIME_SEC 60

#define ROBOT_STATE_LEARNING 3
#define ROBOT_STATE_POLICY 1
#define ROBOT_STATE_CRITIC 2
#define ROBOT_STATE_WANT 0

#define SIMULATOR_STATE_FREE 0
#define SIMULATOR_STATE_BUSY 1

#define ROBOT_LEFT (ROBOT_ID + ROBOT_TOTAL - 1) % ROBOT_TOTAL
#define ROBOT_RIGHT (ROBOT_ID + 1) % ROBOT_TOTAL

#define PROPER_SIMULATOR ROBOT_ID - i

sem_t SERVER_MUTEX;
int SERVER_EXPERIENCE[ROBOT_TOTAL];
int SERVER_REPORT_TOTAL = 0;
int SERVER_REPORT_COUNTS[ROBOT_TOTAL];
double SERVER_REPORT_TIME_COUNTS[ROBOT_TOTAL];

sem_t SIMULATOR_MUTEX[SIMULATOR_TOTAL];
int SIMULATORS[SIMULATOR_TOTAL];
int SIMULATOR_STATES[SIMULATOR_TOTAL];

sem_t ROBOT_MUTEX[ROBOT_TOTAL];
int ROBOT_STATES[ROBOT_TOTAL];
int ROBOTS[ROBOT_TOTAL];

int findProperSimulator(int ROBOT_ID);

const long calculateTimeDiff(time_t time1){
    time_t currentTime = time(NULL);
    return labs((long) currentTime - ((long) time1));
}

const char *getCurrentTimeString(){
    time_t currentTime;
    struct tm *loc_time;

    //Getting current time of system
    currentTime = time(NULL);

    // Converting current time to local time
    loc_time = localtime(&currentTime);

    // Displaying date and time in standard format
    char *resultTime = asctime(loc_time);
    resultTime[24] = '\0';
    return resultTime;
}

void validateBeforeSimulate(int ROBOT_ID, int SIMULATOR_ID){
    //printf("EVENT: %d validateBeforeSimulate() %d\n", ROBOT_ID, SIMULATOR_ID);

    if(SIMULATOR_ID < 0){
        SIMULATOR_ID = findProperSimulator(ROBOT_ID);
        ROBOT_STATES[ROBOT_ID] = ROBOT_STATE_WANT;
    }

    if(ROBOT_STATES[ROBOT_ID] == ROBOT_STATE_WANT){
        if(ROBOT_LEFT == ROBOT_RIGHT){
            return;
        }

        if(ROBOT_STATES[ROBOT_LEFT] != ROBOT_STATE_WANT){
            return;
        }

        if(ROBOT_STATES[ROBOT_RIGHT] != ROBOT_STATE_WANT){
            return;
        }

        time_t t;
        srand((unsigned) time(&t));

        ROBOT_STATES[ROBOT_RIGHT] = (rand() % 100 < 50) ? ROBOT_STATE_POLICY : ROBOT_STATE_CRITIC;
        ROBOT_STATES[ROBOT_LEFT] = (ROBOT_STATES[ROBOT_RIGHT] == ROBOT_STATE_POLICY) ? ROBOT_STATE_CRITIC
                                                                                     : ROBOT_STATE_POLICY;
        //printf("ROBOT %d: Left= %d, Right= %d\n", ROBOT_ID, ROBOT_LEFT, ROBOT_RIGHT);
        ROBOT_STATES[ROBOT_ID] = ROBOT_STATE_LEARNING;
        SIMULATOR_STATES[SIMULATOR_ID] = SIMULATOR_STATE_BUSY;

        int criticId;
        int policyId;
        if(ROBOT_STATES[ROBOT_LEFT] == ROBOT_STATE_POLICY){
            criticId = ROBOT_RIGHT;
            policyId = ROBOT_LEFT;
        }else{
            criticId = ROBOT_LEFT;
            policyId = ROBOT_RIGHT;
        }

        // Learning Session Starts
        printf("LEARN[%s]\n", getCurrentTimeString());
	printf("LearnerID: %d\n",ROBOT_ID+1);
	printf("HelperID No.1: %d\n",criticId);
	printf("HelperID No.2: %d\n",policyId);
	printf("SimulatorID: %d\n\n",SIMULATOR_ID+1);
	



        time_t startingTime;
        time(&startingTime);

        // Sleep for a defined Period Of time
        sleep(SIMULATION_TIME_SEC);

        // Learning Session Ends
        printf("DONE[%s]\n", getCurrentTimeString());
	printf("LearnerID: %d\n",ROBOT_ID+1);
	printf("HelperID No.1: %d\n",criticId);
	printf("HelperID No.2: %d\n",policyId);
	printf("SimulatorID: %d\n\n",SIMULATOR_ID+1);



        // Enters Critical Region (Server)
        sem_wait(&SERVER_MUTEX);
        // Counts Robot Experience Point
        SERVER_EXPERIENCE[ROBOT_ID] += SIMULATION_TIME_SEC;

        printf("UPDATE[%s]\n", getCurrentTimeString());
	printf("LearnerID: %d\n",ROBOT_ID+1);
	printf("SimulatorID: %d\n\n",SIMULATOR_ID+1);
        // Count how many times server get reports
        SERVER_REPORT_COUNTS[ROBOT_ID]++;
        SERVER_REPORT_TOTAL++;
        SERVER_REPORT_TIME_COUNTS[ROBOT_ID] += calculateTimeDiff(startingTime);

        // Exits Critical Region (Server)
        sem_post(&SERVER_MUTEX);

        sem_post(&ROBOT_MUTEX[ROBOT_ID]);
    }
}

void startLearning(int ROBOT_ID, int SIMULATOR_ID){
    //printf("ROBOT %d: startLearning()\n", ROBOT_ID);

    sem_wait(&SIMULATOR_MUTEX[SIMULATOR_ID]);

    //printf("Robot %d: Acquires Simulator #%d\n", ROBOT_ID, SIMULATOR_ID);

    // Robot wants to LEARN
    ROBOT_STATES[ROBOT_ID] = ROBOT_STATE_WANT;
    validateBeforeSimulate(ROBOT_ID, SIMULATOR_ID);

    sem_post(&SIMULATOR_MUTEX[SIMULATOR_ID]);

    sem_wait(&ROBOT_MUTEX[ROBOT_ID]);
    sleep(1);
}

void stopLearning(int ROBOT_ID, int SIMULATOR_ID){
    sem_wait(&SIMULATOR_MUTEX[SIMULATOR_ID]);

    // Current Robot is now thinking
    ROBOT_STATES[ROBOT_ID] = ROBOT_STATE_WANT;
    SIMULATOR_STATES[SIMULATOR_ID] = SIMULATOR_STATE_FREE;

    // Test its neighbors
    validateBeforeSimulate(ROBOT_LEFT, -1);
    validateBeforeSimulate(ROBOT_RIGHT, -1);

    sem_post(&SIMULATOR_MUTEX[SIMULATOR_ID]);
}

int findProperSimulator(int ROBOT_ID){
    int min = 99;
    for(int i = 0; i < SIMULATOR_TOTAL; i++){
        if(PROPER_SIMULATOR < min){
            min = abs(PROPER_SIMULATOR);
        }
    }
    return min;
}

void *robotRoutine(void *param){
    int *ROBOT_ID = param;
    while(1){
        sleep(1);
        int simulatorId = findProperSimulator(*ROBOT_ID);
        startLearning(*ROBOT_ID, simulatorId);
        sleep(0);
        stopLearning(*ROBOT_ID, simulatorId);
    }
}

void generateReports(){ // Report Master-Mind Server
    for(int i = 0; i < ROBOT_TOTAL; ++i){
        printf("%d: %.2f, %d\n", i+1, SERVER_REPORT_TIME_COUNTS[i], SERVER_REPORT_COUNTS[i]);
    }
    printf("MASTER: %d\n", SERVER_REPORT_TOTAL);
}

int main(){ // need to add int arg , char *argv[]
    int i,j;
    pthread_t threadId[ROBOT_TOTAL];

    printf("Initializing Threads!\n");

    // Establishes Server's Mutual Exclusivity
    sem_init(&SERVER_MUTEX, 0, 1);

    // Initializes the Simulator Mutexes
    for(i = 0; i < SIMULATOR_TOTAL; i++){
        sem_init(&SIMULATOR_MUTEX[i], 0, 1);
        SIMULATORS[i] = i;
    }

    for(i = 0; i < ROBOT_TOTAL; i++){
        ROBOTS[i] = i;
        ROBOT_STATES[i] = ROBOT_STATE_WANT;
        sem_init(&ROBOT_MUTEX[i], 0, 0);
    }

    for(i = 0; i < ROBOT_TOTAL; i++){
        pthread_create(&threadId[i], NULL, robotRoutine, &ROBOTS[i]);
        printf("Robot Number %d is ready!\n", i+1);
	pthread_join(threadId, NULL);
    }

// Need to create Pthread Join by using for loop
//    for(i = 0; i < ROBOT_TOTAL; i++){
//        pthread_join(threadId[i], NULL);
//    }

    sleep(TOTAL_SIMULATION_TIME_SEC);

    printf("Terminating Robots...\n");
    for(i = 0; i < ROBOT_TOTAL; i++){
        pthread_cancel(threadId[i]);
    }

    printf("Termination Completed!\n");

    printf("Generating Reports...\n");
    generateReports();
    return 0;
}
