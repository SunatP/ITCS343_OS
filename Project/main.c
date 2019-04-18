/*
	ITCS343: Principles of Operating Systems, Project 1: Learning Tachikoma
	Sunat P. 6088130 Sec 1
	Nutravee C. 6088016 Sec 1
	Teacher Thanapon Noraset
	Thank for a Dining Philosopher Knowledge from GeeksforGeek.com , thecrazyprogrammer.com , c-program-example.com
	Compiler gcc g++ (Ubuntu 8.2.0-7ubuntu1) 8.2.0 on Ubuntu 18.10
*/
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <bits/sigthread.h>
#include <signal.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdbool.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic pop
//Declare the boolean Statement
#define TRUE 1
#define FALSE 0

#define ROBOT_LEFT (mid + Robot - 1) % Robot       // Define the formula for Calculating Left ROBOT's Position
#define ROBOT_RIGHT (mid +  1) % Robot                    // Define the formula for Calculating RIGHT ROBOT's Position

// Declare Global Variable
int Robot;                            // N = Number of tachikoma robots
int Simulator;                        // M = Number of simulators
int Sleep_Time;                       // E = Maximum period of time that a simulator runs in one playing (in seconds)
int Limited_Time;                     // T = fixed period of time that the whole learning will take (in seconds)

//Declare Robot Status
int Interrupt[300];                   // State to Robot that is interrupt
int Report_Counter = 1;               // Report number counter for each Robot
int RobotGeneratedReport[300];
int SERVER_REPORT_TIME_COUNTS[300];   // Count Report for Master Report
int SERVER_REPORT_MASTER;   // Count Report for Master Report
int CurrentJob[300];                  // Check Robot Status

//Declare Robot Time Used
long long Start_Time;                 //Initialize system time
long long LearnTime[300];
long long StartLearningTime[300];

static pthread_mutex_t RobotStatus;     // Declare an object of type mutex for checking Robot status
static pthread_mutex_t Report_Mutex;    // Declare an object of type mutex for report
static pthread_mutex_t Machine_Status;  // Declare an object of type mutex for checking lock or unlock status Simulator

struct timespec START_TIME;             // Specifies a time in seconds and nanoseconds

sem_t *simulator;                       // Semaphore datatype for avaliable Machine

long long current_timestamp(time_t inputTime)
{
  time_t currentTime = time(NULL); // Initialize Time type for CurrentTime at 0
   return labs((long) currentTime - ((long) inputTime)); // labs() is long int datatype of abs() function
}

uint64_t deltaTimeInMicrosecond()
{
    struct timespec current; // Specifies a time for the current time
    clock_gettime(CLOCK_MONOTONIC_RAW, &current); // CLOCK_MONOTONIC_RAW is represents the absolute elapsed wall-clock time since some arbitrary
    // clock_gettime is function gets the current time of the clock specified by clock_id, and puts it into the buffer pointed to by tp
    return (uint64_t) ((current.tv_sec - START_TIME.tv_sec) * 1000000 + (current.tv_nsec - START_TIME.tv_nsec) / 1000); // tv_sec for seconds , tv_nsec for nanoseconds
    // uint64_t is unsigned integer type with width of exactly 64 bits

}

int Find_Robot()
{
    int target = -1;
    for(int i=0;i<Simulator;i++){
        if(!Interrupt[i]) target = i;        // Return the machine state if avaliable
      }
    return target;
}

void *Start_Session(void* machineid)
{
    int mid = (long) machineid; // Convert long int to int
    bool Learn = false; // Declare Variable boolean type
    int Find_Simulator;
    int left = ROBOT_LEFT;
    int right = ROBOT_RIGHT;

    while(TRUE)
    {
        if(deltaTimeInMicrosecond()/1000000 >= Limited_Time) // If elapsed time is more than input Limited_Time
        {
          pthread_exit(0); // Terminates calling threads while the blocks can allocated
          //exit(0); is terminate program
          //return 0; is equal to pthread_exit but Stackoverflow say
          /* "valgrind --leak-check=full --show-reachable=yes shows 5 blocks allocated
          from functions called byvalgrind --leak-check=full --show-reachable=yes
          shows 5 blocks allocated from functions called by
          pthread_exit() that is unfreed but still reachable at process exit.
          If the pthread_exit(0); is replaced by return 0;, the 5 blocks are not allocated." */
        }
         if(!Learn)
        { // If Robot can Learn
            pthread_mutex_lock(&RobotStatus); // Block RobotStatus until the mutex becomes available
            if(CurrentJob[mid] == 0 && CurrentJob[left] != -1 && CurrentJob[right] != -1) // If Robot is free and other Robot is not learning

            {
                CurrentJob[mid] = -1;
                CurrentJob[left]++;           // Set Robot left hand to busy helping
                CurrentJob[right]++;          // Set Robot right hand to busy helping
                Learn = true;                 // Robot Avaliable to Learn
            }
            pthread_mutex_unlock(&RobotStatus); // Unlock Mutex name RobotStatus
        }

        if(!Learn) continue;  // If Robot can learn again then continue

        sem_wait(simulator);    // Decrement the Semaphore name simulator
        pthread_mutex_lock(&Machine_Status);  // Block Machine_Status until the mutex becomes available
        Find_Simulator = Find_Robot();  // Find the available Simulator
        while(Find_Simulator== -1);
        Interrupt[Find_Simulator] = true; // Simulator is interrupt
        pthread_mutex_unlock(&Machine_Status); // Unlock mutex name Machine_Status
        time_t startingTime = time(NULL); // Initialize Time type for CurrentTime at 0

            //Start Learning
            StartLearningTime[mid] = deltaTimeInMicrosecond()/1000000 ; // Convert Time from Microsecond to seconds
            printf("LEARN[%" PRIu64 "]: %d,  %d, %d\n",deltaTimeInMicrosecond(),(int)mid,left,right);
                fflush(stdout);
            // PRIu64 will still show the Microsecond
            sleep(Sleep_Time); // Delay with input from argv[3]

            LearnTime[mid] += current_timestamp(startingTime); // Get Robot learntime from 0

            printf("DONE[%" PRIu64 "]:  %d,  %d,  %d\n",deltaTimeInMicrosecond(),(int)mid,left,right);
                fflush(stdout);

            if(deltaTimeInMicrosecond()/1000000  <= Limited_Time) // If Time Less or Equal to intput from argv[4]
            {
                // Update Robot Status
                pthread_mutex_lock(&Report_Mutex);  // Block Report_Mutex until the mutex becomes available
                printf("UPDATE[%" PRIu64 "]:  %d,  %d\n",deltaTimeInMicrosecond(),(int)mid,Report_Counter);
                    fflush(stdout);
                RobotGeneratedReport[mid]++; // Increment
                SERVER_REPORT_TIME_COUNTS[mid]+= current_timestamp(startingTime);  // Count the Duration each Robot takes to learn
                Report_Counter++;
                SERVER_REPORT_MASTER++;
                pthread_mutex_unlock(&Report_Mutex);             // Unlock mutex name Report_Mutex
            }

            //Learn Complete
            pthread_mutex_lock(&Machine_Status);                      // Block Machine_Status untill the mutex becomes available
            Interrupt[Find_Simulator] = false;                        // Simulator status is available
            pthread_mutex_unlock(&Machine_Status);                    // Unlock mutex name Machine_Status
            pthread_mutex_lock(&RobotStatus);                         // Block RobotStatus until the mutex becomes available
            CurrentJob[mid] = 0;
            CurrentJob[left]--;           // Set Robot left hand to busy helping
            CurrentJob[right]--;          // Set Robot Right hand to busy helping
            pthread_mutex_unlock(&RobotStatus); // Unlock mutex name RobotStatus
            sem_post(simulator);          // Find Simulator that Robot can use it
            Find_Simulator=-1;
            Learn = false;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int Robot_ID;

    if(argc != 5)       // Create if else condition to aware the user forgot to input the value from Terminal
    {
        fprintf(stderr, "USAGE:./main <INT> <INT> <INT> <INT>\n");
        printf("Example: ./main 5 3 1 10\n");
        exit(-1);
    }

    Robot = atoi(argv[1]); // atoi = ASCII to Integer with receive value from argv[1]
    pthread_t Robot_T[Robot];
    Simulator = atoi(argv[2]);
    Sleep_Time = atoi(argv[3]);
    Limited_Time = atoi(argv[4]);


    simulator = sem_open("/SIMULATOR", O_CREAT, 0666, Simulator); // initialize and open a named simulator semaphore
    sem_close(simulator); // closes the named simulator semaphore
    sem_unlink("/SIMULATOR"); // removed simulator semaphore from the system only when the reference count reaches 0
    simulator = sem_open("/SIMULATOR", O_CREAT, 0666, Simulator); // initialize and open a named simulator semaphor

    pthread_mutex_init(&RobotStatus, NULL);     // Initialize the RobotStatus mutex referenced by mutex with attributes
    pthread_mutex_init(&Machine_Status, NULL); // Initialize the Machine_Status mutex referenced by mutex with attributes
    pthread_mutex_init(&Report_Mutex, NULL);  // Initialize the Report_Mutex mutex referenced by mutex with attributes
    clock_gettime(CLOCK_MONOTONIC_RAW, &START_TIME);      // Mark the starting time of the program

    Start_Time = deltaTimeInMicrosecond()/1000000 ;       // Clock start count after run program.


    for(Robot_ID = 0; Robot_ID<Robot; Robot_ID++)
    {
      pthread_create(&Robot_T[Robot_ID], NULL, Start_Session, (void*)(intptr_t)Robot_ID); // Create a new thread Robot_T by using Start_Session
    }

    for(Robot_ID = 0; Robot_ID<Robot; Robot_ID++)
    {
        pthread_join(Robot_T[Robot_ID],NULL); // Suspend execution of the calling Robot_T thread until the target thread terminates
    }

    for(int i = 0; i<Robot; i++)
    {
        printf("%d: %d, %d\n",i,SERVER_REPORT_TIME_COUNTS[i],RobotGeneratedReport[i]);
    }
    /* fflush() is function is typically used for output stream only.
    Its purpose is to clear (or flush) the output buffer and move the buffered data to console*/
    //printf("Master: %d\n",SERVER_REPORT_MASTER-2);
    pthread_mutex_lock(&Report_Mutex); // Block Report_Mutex untill the mutex becomes available
    printf("MASTER: %d\n",Report_Counter-1);
    fflush(stdout);
    pthread_mutex_unlock(&Report_Mutex); // Unlock pthread name Report_Mutex
    sleep(1);
    return 0;
}
