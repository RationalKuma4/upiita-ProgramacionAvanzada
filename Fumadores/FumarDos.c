#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

int cerillo = 0;
int papel = 1;
int tabaco = 2;

sem_t semFum1Cerillo;
sem_t semFum2Papel;
sem_t semFum3Tabaco;
sem_t semAgente;
int Ing1 = 0;
int Ing2 = 0;
int Ing3 = 0;

/*V();
P();*/
void *Fumador1Cerillo();
void *Fumador2Papel();
void *Fumador3Tabaco();
void *Agente();

int main()
{	
	pthread_t tA, tC, tP, tT;	
	
	sem_init(&semFum1Cerillo, 0, 0);
	sem_init(&semFum2Papel, 0, 0);
	sem_init(&semFum3Tabaco, 0, 0);
	sem_init(&semAgente, 0, 0);

	pthread_create(&tA, NULL, &Agente, NULL);
	pthread_create(&tC, NULL, &Fumador1Cerillo, NULL);
	pthread_create(&tP, NULL, &Fumador2Papel, NULL);
	pthread_create(&tT, NULL, &Fumador3Tabaco, NULL);

	Agente();
	return 0;
}

void *Fumador1Cerillo()
{
	sem_wait(&semFum1Cerillo);
	printf("Soy fumador y tengo cerillo \n");
	sleep(5);
	printf("Termina fumador\n");
	sem_post(&semAgente);
}

void *Fumador2Papel()
{
	sem_wait(&semFum2Papel);
	printf("Soy fumador y tengo papel \n");
	sleep(5);
	printf("Termina fumador\n");
	sem_post(&semAgente);
}

void *Fumador3Tabaco()
{
	sem_wait(&semFum3Tabaco);
	printf("Soy fumador y tengo tabaco \n");
	sleep(5);
	printf("Termina fumador\n");
	sem_post(&semAgente);
}


void *Agente()
{
	while(1)
	{
		do
		{	
			Ing1=rand()% 3;
			Ing2=rand()% 3;
			printf("Ingredientes generados: %d, %d \n", Ing1, Ing2);
		}
		while (Ing1==Ing2);
		printf("Ingredientes finales: %d, %d \n", Ing1, Ing2);
		sleep(3);
		if ((Ing1==cerillo && Ing2==papel) || (Ing1==papel && Ing2==cerillo))
		{
			printf("Soy agente y genere cerillo y tabaco \n");
			sem_post(&semFum3Tabaco);
			sem_wait(&semAgente);
		}
		else if ((Ing1==cerillo && Ing2==tabaco)||(Ing1==tabaco && Ing2==cerillo))
		{
			printf("Soy agente y genere cerillo y tabaco \n");
			sem_post(&semFum2Papel);
			sem_wait(&semAgente);
		}
		else if ((Ing1==papel && Ing2==tabaco)||(Ing1==tabaco && Ing2==papel))
		{
			printf("Soy agente y genere cerillo y tabaco \n");
			sem_post(&semFum1Cerillo);
			sem_wait(&semAgente);
		}
	}
}
