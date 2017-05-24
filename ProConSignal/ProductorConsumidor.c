#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
//#include <font1.h>

#define MAX_BUFFER 10
#define DATOS_A_PRODUCIR 100

/*sem_t elementos;
sem_t huecos;*/


int buffer[MAX_BUFFER];
void* Productor(void* var);
void* Consumidor(void* var); 
int static tamBuf = MAX_BUFFER;
pthread_t th1, th2;
pthread_t   tipro, ticon;
void main()
{
	/*sem_init(&elementos,0,0);
	sem_init(&huecos,0,MAX_BUFFER);*/

	pthread_create(&th1, NULL, Productor, NULL);
	pthread_create(&th2, NULL, Consumidor, NULL);
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	/*sem_destroy(&huecos);
	sem_destroy(&elementos);*/

	exit(0);
}

void* Productor(void* var)
{
	while(1)
	{
		tipro = pthread_self();
		printf("%u \n", tipro);
		int pos=0;
		int dato;
		int i;
		for(i=0; i<DATOS_A_PRODUCIR; i++)
		{
			printf("Tamano buffer %d\n", tamBuf);
			sleep(0);
			if(tamBuf == 0)
			{
				printf("Buffer lleno, paro de producir\n");
				kill(19, tipro);
				break;
			}
			tamBuf--;
			dato=i;
			printf("Producioendo %d\n", i);

			/*sem_wait(&huecos);*/

			buffer[pos]=i;
			pos=(pos+1)%MAX_BUFFER;

			/*sem_post(&elementos);*/
		}
	}
	pthread_exit(0);
}

void* Consumidor(void* var)
{
	while(1)
	{
		ticon = pthread_self();
		int pos=0;
		int dato;
		int i;
		for(i=0; i<DATOS_A_PRODUCIR; i++)
		{
			printf("Tamano buffer %d \n", tamBuf);
			sleep(0);
			if(tamBuf == 10)
			{	
				printf("Buffer vacio, dejo de comer y despierto a prodcutor\n");
				kill(19, ticon);
				kill(18, tipro);
				break;
			}
			tamBuf++;
			printf("Consumiendo %d\n", i);

			/*sem_wait(&elementos);*/

			dato=buffer[pos];
			pos=(pos+1)%MAX_BUFFER;

			/*sem_post(&huecos);*/

		}
	}
	pthread_exit(0);
}
