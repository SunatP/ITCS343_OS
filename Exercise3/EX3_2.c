#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Size of array
#define x 7

// Max number of thread
#define Th_x 3

// Array
int a[x] = {90,81,78,95,79,72,85};

// Array to store max number of threads
int x_num[Th_x] = { 0 };
int thread_no = 0;

//Find average
void average(void* arg){
    int i,num=thread_no++;
    int avg=0;
    for(i=0;i<x;i++){
        avg=avg+a[i];
    }
    avg=avg/x;
    x_num[num]=avg;
}

//Find maximum
void maximum(void* arg){
    int i,num=thread_no++;
    int max=0;
    for(i=0;i<x;i++){
        if(a[i]>max){
           max=a[i];
        }
    }
    x_num[num]=max;
}

//Find minimum
void minimum(void* arg){
    int i,num=thread_no++;
    int min=10000;
    for(i=0;i<x;i++){
        if(a[i]<min){
            min=a[i];
        }
    }
    x_num[num]=min;
}

int main(){
    int avg=0,max=0,min=1000;

    int i;
    pthread_t threads[Th_x];

    // creating 3 threads
    for(i=0; i<Th_x; i++){
        pthread_create(&threads[i], NULL, average, (void*)NULL);
    }

    // all 3 threads to complete
    for(i=0; i<Th_x; i++){
        pthread_join(threads[i], NULL);
    }

    // Finding average by individual threads
    for(i=0; i<Th_x; i++){
        avg=x_num[i];
    }


    //-------

    thread_no = 0;

    for(i=0; i<Th_x; i++){
        pthread_create(&threads[i], NULL, maximum, (void*)NULL);
    }

    for(i=0; i<Th_x; i++){
        pthread_join(threads[i], NULL);
    }


    for(i=0; i<Th_x; i++){
        if(x_num[i]>max){
            max=x_num[i];
        }
    }


    //---------

    thread_no = 0;

    for(i=0; i<Th_x; i++){
        pthread_create(&threads[i], NULL, minimum, (void*)NULL);
    }

    for(i=0; i<Th_x; i++){
        pthread_join(threads[i], NULL);
    }


    for(i=0; i<Th_x; i++){
        if(x_num[i]<min){
            min=x_num[i];
        }
    }
    printf("The average value is %d\n", avg);
    printf("The maximum value is %d\n", max);
    printf("The minimum value is %d", min);

    return 0;
}
