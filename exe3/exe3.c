#include <pthread.h>
#include <stdio.h>
void *compute_x(void *x_void_ptr) 
{int *x_ptr = (int *)x_void_ptr;
while(++(*x_ptr) < 100);
printf("value of xis%d \n", *x_ptr);
/* the function returnsNULL */
return NULL;
}
void *compute_y (void *y_void_ptr) 
{
int *y_ptr = (int *) y_void_ptr;
while ((*y_ptr)<2000)*y_ptr = *y_ptr*2;
printf ("value of yis %d \n", *y_ptr); 
/* the function returnsNULL */
return NULL;
}int main(){int x = 0, y = 1; 
/* show the initial values of x and y */
pthread_t x_thread, y_thread;
/* create a thread which executes compute_x*/
if(pthread_create(&x_thread, NULL, compute_x,&x)) 
{
fprintf(stderr, "Error creating thread\n");
return 1;
}
/* create a thread which executes compute_y */
if (pthread_create (&y_thread, NULL, compute_y,&y)) 
{
fprintf(stderr, "Error creating thread\n");
return 1;
}
/* wait for two threadsto finish */
pthread_join(x_thread, NULL);
pthread_join (y_thread, NULL);
printf("x: %d, y: %d\n", x, y);
return 0;
}
