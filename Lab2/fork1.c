#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#define max 5

void childprocess (pid_t pid);
void parentprocess (pid_t pid);

void main (void)
{
	pid_t pid;	
	pid = fork ();
	if (pid == 0) childprocess (pid);
	else parentprocess (pid);
}
void childprocess (pid_t pid)
{
int i;
for (i=0; i < max; i++)
printf (" this line is from child with pid %d, value = %d\n", pid, i);
printf (" child process is done \n");
}
void parentprocess (pid_t pid)
{
int i;
for (i=0; i < max; i++)
printf (" this line is from parent with pid %d, value = %d\n", pid, i);
printf("parent process is done \n");
wait (NULL);
printf ("child process is finished \n");
}

