#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid, i;
FILE* archivo;
const char* nombre = "Archivo.txt";
const char* escribir = "w+";

void EscribirArchivo(char* cadena);
void LeerArchivo();

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
		}
	}
}

void EscribirArchivo(char* cadena)
{
	archivo = fopen(nombre, escribir);
	fprintf(archivo, "%s", cadena);
	fclose(archivo);
}

void LeerArchivo()
{
	int palabra = 0;
	archivo = fopen(nombre, "r");
	while (1)
	{
		palabra = fgetc(archivo);
		if (feof(archivo))
		{
			break;
		}
		printf("%c", palabra);
	}
}
