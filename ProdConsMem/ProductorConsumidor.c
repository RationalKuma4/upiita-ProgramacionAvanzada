#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <sys/shm.h>

#define MAX_BUFFER 10
#define DATOS_A_PRODUCIR 100

sem_t mutex;
int buffer[MAX_BUFFER];
void* Productor(void* var);
void* Consumidor(void* var); 
void ImprimeBuffer();
void IniciaBuffer();
void CrearMemoriaCompartida();
int ContarElementosBuffer();

int static tamBuf = MAX_BUFFER;
pthread_t th1, th2;
pthread_t   tipro, ticon;

/*Variables de memoria compartida*/
void CrearMemoriaCompartida();
void LeerMemoriaCompartida();
void LiberarMemoriaCreada();
void LiberarMemoriaLeida();
int *Memoria = NULL;
int Id_Memoria;

void main(int argc, char **argv)
{
	int num = atoi(argv[1]);
	sem_init(&mutex,0,1);
	
	//Productor
	if(num==1)
	{
		CrearMemoriaCompartida();
		pthread_create(&th1, NULL, Productor, NULL);
		pthread_join(th1, NULL);
	}
	//Cinsumidor
	else if(num==2)
	{
		LeerMemoriaCompartida();
		pthread_create(&th2, NULL, Consumidor, NULL);
		pthread_join(th2, NULL);
	}


	if(num==1)
	{
		LiberarMemoriaCreada();
	}
	//Cinsumidor
	else if(num==2)
	{
		LiberarMemoriaLeida();
	}

	sem_destroy(&mutex);

	exit(0);
}

void* Productor(void* var)
{
	while(1)
	{
		tipro = pthread_self();
		int pos=0;
		int dato;
		int i;
		for(i=0; i<DATOS_A_PRODUCIR; i++)
		{
			sleep(1);
			printf("%d\n", ContarElementosBuffer());
			sem_wait(&mutex);
			if(ContarElementosBuffer() >= 9)
			{
				printf("Buffer lleno, paro de producir\n");
				kill(19, tipro);
				sem_post(&mutex);
				break;
			}
			
			tamBuf--;
			dato=i;
			printf("Producioendo %d\n", i);
			Memoria[pos]=i;
			pos++;
			ImprimeBuffer();
			sem_post(&mutex);
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
			sleep(2);
			printf("%d\n", ContarElementosBuffer());
			sem_wait(&mutex);

			if(ContarElementosBuffer() <= 0)
			{	
				printf("Buffer vacio, dejo de comer y despierto a prodcutor\n");
				kill(18, tipro);
				kill(19, ticon);
				sem_post(&mutex);
				break;
			}

			tamBuf++;
			printf("Consumiendo %d\n", i);
			Memoria[pos]=0;
			pos++;			
			ImprimeBuffer();
			sem_post(&mutex);
		}
	}
	pthread_exit(0);
}

void ImprimeBuffer()
{
	for(int i=0; i<9; i++)
	{
		printf("%d ", Memoria[i]);
	}
	printf("\n");
}

void IniciaBuffer()
{
	for(int i=0; i<9; i++)
	{
		Memoria[i] = 0;
	}
}

void CrearMemoriaCompartida()
{
	key_t Clave;
	int j;

	//Se consigue clave para la memoria compartida
	Clave = ftok ("/bin/ls", 33);
	if (Clave == -1)
	{
		printf("No consigo clave para memoria compartida\n");
		exit(0);
	}

	//Se crea la memoria compartida
	Id_Memoria = shmget (Clave, sizeof(int)*100, 0777 | IPC_CREAT);
	
	if(Id_Memoria == -1)
	{
		printf("No consigo Id para memoria compartida\n");
		exit(0);
	}
	//Hacemos apuntar hacia la zona de memoria compartida
	Memoria = (int *)shmat (Id_Memoria, (char *)0, 0);
	if(Memoria == NULL)
	{
		printf("No consigo memoria compartida\n");
		exit(0);
	}
	//Escribemos en la memoria compartida
	printf("Se creo la region de memoria\n");
	IniciaBuffer();
	/*for(j=0; j<100; j++)
	{
		Memoria[j] = j;
		printf("Se escribió %i\n",j);
		sleep(1);
	}*/
}

void LeerMemoriaCompartida()
{
	key_t Clave;
	int i;

	//Se obtine la clave para la memoria compartida
	Clave=ftok("/bin/ls",33);
	if(Clave==-1){
		printf("No consigo clave para memoria compartida\n");
		exit(0);
	}
	
	//Se obtine un id para la memoria compartida
	Id_Memoria=shmget(Clave,sizeof(int)*100,0777);
	if(Id_Memoria==-1){
		printf("No consigo Id para memoria compartida\n");
		exit(0);
	}

	//Se obtine puntero para la memoria compartida
	Memoria=(int *)shmat(Id_Memoria, (char *)0,0);
	if(Memoria==NULL){
		printf("No consigo memoria compartida");
		exit(0);
	}	
	printf("Se puede leer la memoria\n");
	ImprimeBuffer();
	//Se lee de la memoria compartida
	/*for(i=0; i<100; i++){
		printf("Leyendo de la memoria %i\n",Memoria[i]);
		sleep(1);
	}*/
}

void LiberarMemoriaCreada()
{
	shmdt((char * )Memoria);
	shmctl(Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);
}

void LiberarMemoriaLeida()
{
	//Desasociamos nuestro puntero de la memoria compartida.
	if(Id_Memoria !=-1){
		shmdt((char *)Memoria);
	}
}

int ContarElementosBuffer()
{
	int elementos=0;
	for(int i=0; i<10; i++)
	{
		if(Memoria[i] != 0)
			elementos=elementos+1;
	}
	return elementos;	
}
