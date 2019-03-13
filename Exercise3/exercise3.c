#include<pthread.h>
#include<stdio.h>
#include<stdlib.>

#define max_size 7 // Array size

#define p_thread 3 // Max process thread
void *average(void* arg);
void *maximum(void* arg);
int a[max_size] = {90,81,78,95,79,72,85};

double average;
int fmin;
int fmax;
int num_thread[p_thread] = {0};
int thread_no = 0;


void average(void* arg)
{
  int i;
  int num= thread_no++;
  double avg;
  for(i=0;i<max_size;i++)
  {
    avg = avg + a[i];ss
  }
  avg= avg/max_size;
  num_thread[num] = avg;

}

void maximum(void* arg)
{
  int i;
  int num= thread_no++;
  int max;
  for(i=0;i<max_size;i++)
  {
    if (a[i]>max) {
      max = a[i];
    }
  }
  num_thread[num] = max;
}

void minimum(void* arg)
{
  int i;
  int num= thread_no++;
  int min = 9999999;
  for(i=0;i<max_size;i++)
  {
    if (a[i]<min) {
      min = a[i];
    }
  }
  num_thread[num] = min;
}
