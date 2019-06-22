#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/time.h>
int Time_report =0;  //Initial Time Report = 0;
int readcount = 0, writecount = 0,mainSleepTime=1 ,data =0,x,y;
sem_t rmutex, wmutex , readTry , resource ,downeadTry; // Initial Semaphore Variable
long long program_start_time;                 //Initial system time
long long usetime[10];						// Create long usetime with array that can contain 10 value
long long StartTime[10];				// Create long StartTime with array that can contain 10 value
struct tm * timeinfo;					// Create Structure

long long current_timestamp() { // Get Current Time
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

double elapsedtime() {
    long long diff = current_timestamp() - program_start_time; // Calulate Different Time From Start To End
    double ms = diff%1000;
    double sec = diff/1000;
    return sec + ms/1000;
}
/*
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
}*/

void *reader(void *arg) {
  long midL = (long) arg;
  int mid = (int) midL;   // Convert from long value to Integer Value
  usetime[mid] = current_timestamp();
    sleep((rand()%100)/100.0*(x+y)); // Sleep by using Random value mod with 100 and divide by 100.0(double) and multiply by (x and y) Value
    int n = ((int)arg);
    sem_wait(&readTry);		
    sem_wait(&rmutex);		
    readcount++;			// Increase readcount by 1 per time

 if (readcount == 1) // if readcount equal to 1
    sem_wait(&resource);	// If the value of the semaphore is negative, the calling process blocks; one of the blocked processes wakes up when another process calls sem_post.

sem_post(&rmutex); // wakes up a blocked process waiting on the semaphore 

sem_post(&readTry); // wakes up a blocked process waiting on the semaphore 


printf("Data read by the reader %d is %d\n",n,data);
StartTime[mid] += current_timestamp() - usetime[mid]; // Start time increment or equal 
printf("Timestamp: %.3f seconds\n",elapsedtime());
sem_wait(&rmutex);

readcount--; // Decrement readcount by 1 per time

if (readcount == 0)  // if readcount equal to 0
    sem_post(&resource); // wakes up a blocked process waiting on the semaphore ปลุก process ขึ้นมา
sem_post(&rmutex); // wakes up a blocked process waiting on the semaphore

void *writer(void *arg) {
  long midL = (long) arg;
  int mid = (int) midL;  //แปลงค่าจาก long เป็น int
  usetime[mid] = current_timestamp(); // เวลาที่ใช้ = เวลาปัจจุบัน
    sleep((rand()%100)/100.0*(x+y)); // สุ่มค่าจากฟังก์ชัน rand()
    int n = ((int)arg);
    sem_wait(&wmutex); // ถ้ามีสถานะ "ว่าง" อยู่ ก็ทำงานต่อได้ แต่ถ้า "ไม่ว่าง" ก็จะต้องรอ ในกรณีที่เข้าใช้งานได้ Thread ดังกล่าว จะถูกเปลี่ยนจากสถานะ Running เป็น Waiting
    writecount++; 
    data++;

if (writecount == 1)
    sem_wait(&readTry);
sem_post(&wmutex);


sem_wait(&resource);
//<critical section>
printf("Data written by the writer %d is %d\n",n+1,data+1);
printf("Timestamp: %.3f seconds\n",elapsedtime());


//<Exit section>
sem_post(&resource);


    sem_wait(&wmutex);  // ถ้ามีสถานะ "ว่าง" อยู่ ก็ทำงานต่อได้ แต่ถ้า "ไม่ว่าง" ก็จะต้องรอ ในกรณีที่เข้าใช้งานได้ Thread ดังกล่าว จะถูกเปลี่ยนจากสถานะ Running เป็น Waiting
    writecount--;

if (writecount == 0)
    sem_post(&readTry);  // wakes up a blocked process waiting on the semaphore ปลุก process ขึ้นมา
sem_post(&wmutex);  // wakes up a blocked process waiting on the semaphore ปลุก process ขึ้นมา


Time_report++;
}

int main(int argc, char *argv[]) {

srand(time(NULL));
x = atoi(argv[1]);
y = atoi(argv[2]);
program_start_time = current_timestamp();
if(argc != 3)
{
  fprintf(stderr, "Follow this Instruction >> ./main.c <INT> <INT>\n");
  exit(-1);
}
printf("The number input of writers are %d and readers are %d \n",x,y);

pthread_t writetid[x],readtid[y];

void *reader(void *x);
void *writer(void *y);

sem_init(&rmutex,0,1); // ทำการเริ่มโปรเซส ของ semaphore ที่ชื่อ rmutex (rmutex คือ Reader Mutex)
sem_init(&wmutex,0,1); // ทำการเริ่มโปรเซส ของ semaphore ที่ชื่อ wmutex (wmutex คือ Writer Mutex)
sem_init(&resource,0,1);  // ทำการเริ่มโปรเซส ของ semaphore ที่ชื่อ resouce 
sem_init(&readTry,0,1);  // ทำการเริ่มโปรเซส ของ semaphore ที่ชื่อ readTry
int i;
for(i=0;i<x;i++)
{
   pthread_create(&writetid[i],NULL,writer,(void *)i); // สร้าง Pthread โดยใช้ ค่าที่รับมาจาก x
}
for(i=0;i<y;i++)
{

   pthread_create(&readtid[i],NULL,reader,(void *)i); // สร้าง Pthread โดยใช้ ค่าที่รับมาจาก y
}

sleep(x+y); // ใช้การดีเลย์ของงโปรเซส โดยใช้เวลาจาก x+y

printf("Exit the program\n");
exit(0);

 }
