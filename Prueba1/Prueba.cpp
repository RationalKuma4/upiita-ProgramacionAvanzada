#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid, i;
int myArray[3];

void main()
{
	printf("%d \n", getpid());
	for (i = 0; i<4; i++)
	{
		pid = fork();
		if (pid<0)
			printf("Ocurrion un error\n");
		else if (pid == 0)
		{
			printf("Es el proceso hijo %d \n", getpid());
			//break;
			//printf("Mi papa es %d \n", getppid());
		}
		else if (pid>0)
		{
			wait(&pid);
			printf("Codigo del proceso papa %d \n", pid);
			break;
			//printf("Codigo del proceso papa %d \n", getpid());
			//printf("Codigo del proceso bash %d \n", getpid());
		}

	}
	/*while(1)
	{
	printf("En while %d \n", getpid());
	}*/
}
