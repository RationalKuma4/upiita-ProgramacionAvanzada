#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

sem_t duena, raul, annie, puno, pdos, pita, juan, cliente;
pthread_t tDuena, tRaul, tAnnie, tPuno, tPdos, tPita, tJuan;
pthread_t clientes[5];
pthread_t fila[3];
int estadopUno = 0, estadopDos = 0;
int estadoDuena = 0, estadoRaul = 0;
 

void *Cliente(void *a);
void *CortarCabello(void *a);
void *CortarCabelloRaul(void *a);
void *LavarCabello(void *a);
void *PintarCabello(void *a);
void *PintarCabelloEdos(void *a);
void *SecadoPeinadoCabello(void *a);
void *PedicureManicure(void *a);
void *DespertarEmpleado(void *a);
void *VerificaFila(void *a);
void *VerificaDisponibilidad(void *a);
void *FormarCliente(void *a);


int main()
{
	sem_init(&duena, 0, 0);
	sem_init(&raul, 0, 0);
	sem_init(&annie, 0, 0);
	sem_init(&puno, 0, 0);
	sem_init(&pdos, 0, 0);
	sem_init(&pita, 0, 0);
	sem_init(&juan, 0, 0);
	sem_init(&cliente, 0, 0);

	pthread_create(&tDuena, NULL, CortarCabello, NULL);
	pthread_create(&tRaul, NULL, CortarCabelloRaul, NULL);
	pthread_create(&tAnnie, NULL, LavarCabello, NULL);
	pthread_create(&tPuno, NULL, PintarCabello, NULL);
	pthread_create(&tPdos, NULL, PintarCabelloEdos, NULL);
	pthread_create(&tPita, NULL, SecadoPeinadoCabello, NULL);
	pthread_create(&tJuan, NULL, PedicureManicure, NULL);
	

	for (int i = 0; i < 5; i++)
	{
		pthread_create(&clientes[i], NULL, Cliente, NULL);
	}

	for(int i = 0; i < 5; i++)
	{
		pthread_join(clientes[i], NULL);
	}
	
	pthread_join(tDuena, NULL);
	pthread_join(tRaul, NULL);
	pthread_join(tAnnie, NULL);
	pthread_join(tPuno, NULL);
	pthread_join(tPdos, NULL);
	pthread_join(tPita, NULL);
	pthread_join(tJuan, NULL);

	sem_destroy(&duena);
	sem_destroy(&raul);
	sem_destroy(&annie);
	sem_destroy(&puno);
	sem_destroy(&pdos);
	sem_destroy(&pita);
	sem_destroy(&juan);
	exit(0);
	return 0;
}

void *Cliente(void *a)
{
	while (1)
	{
		int opc;
		printf("Menu\n");
		printf("1. Corte de cabello\n");
		printf("2. Pintar cabello\n");
		printf("3. Pedicure/Manicure\n");
		scanf("%d", &opc);
		sem_wait(&cliente);
		printf("\n");
		switch (opc)
		{
		case 1:
			if(estadoDuena == 0)
			{
				estadoDuena = 1;
				sem_post(&duena);
				estadoDuena = 0;
			}
			else
			{
				estadoRaul = 1;
				sem_post(&raul);
				estadoRaul = 0;
			}
			break;
		case 2:
			if(estadopUno == 0)
			{
				estadopUno = 1;
				sem_post(&puno);
				estadopUno = 0;
			}
			else
			{
				estadopDos = 1;
				sem_post(&pdos);
				estadopDos = 0;
			}
			break;
		case 3:
			sem_post(&juan);
			break;
		default:
			printf("Opcion invalida\n");
		}
		sem_post(&cliente);
	}
}

void *PintarCabello(void *a)
{
	while (1)
	{
		sem_wait(&puno);
		printf("Estoy pintando\nTermine\n");
		sleep(3);
		sem_post(&puno);
		sem_post(&annie);
	}
	pthread_exit(0);
}

void *PintarCabelloEdos(void *a)
{
	while (1)
	{
		sem_wait(&pdos);
		printf("Estoy pintando\nTermine\n");
		sleep(3);
		sem_post(&pdos);
		sem_post(&annie);
	}
	pthread_exit(0);
}

void *CortarCabello(void *a)
{
	while (1)
	{
		sem_post(&pita);
		sem_post(&annie);
		sem_wait(&duena);
		printf("Cortando cabello\n");
		sleep(3);
		printf("Termine\n");
		sem_post(&duena);
		sem_post(&pita);
		printf("Orden Terminada\n");
	}
	pthread_exit(0);
}

void *CortarCabelloRaul(void *a)
{
	while (1)
	{
		sem_post(&annie);
		sem_wait(&raul);
		printf("Cortando cabello\n");
		sleep(3);
		printf("Termine\n");
		sem_post(&raul);
		sem_post(&pita);
		printf("Orden Terminada\n");
	}
	pthread_exit(0);
}

void *LavarCabello(void *a)
{
	while (1)
	{
		sem_wait(&annie);
		printf("Estoy lavando cabello\n");
		sleep(3);
		printf("Ya termine\n");
		sem_post(&annie);
	}
	pthread_exit(0);
}

void *SecadoPeinadoCabello(void *a)
{
	while (1)
	{
		sem_wait(&pita);
		printf("Estoy secando y peinando cabello\n");
		sleep(3);
		printf("Ya termine\n");
		sem_post(&pita);
	}
	pthread_exit(0);
}

void *PedicureManicure(void *a)
{
	while (1)
	{
		sem_wait(&juan);
		printf("Estoy trabajando\nTermine\n");
		sleep(3);
		sem_post(&juan);
	}
	pthread_exit(0);
}
