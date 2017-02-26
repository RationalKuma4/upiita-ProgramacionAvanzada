#include <stdio.h>
#include <stdlib.h>

FILE* archivo;
const char* nombre = "Archivo.txt";
const char* escribir = "w+";

void EscribirArchivo(char* cadena);
void LeerArchivo();

int main()
{
	char* palabra;
	scanf(&palabra);
	EscribirArchivo(palabra);
	LeerArchivo();

	return(0);
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
