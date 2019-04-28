#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"
#include "lista.h"

#define TAMANIO_INICIAL_BUFFER 10

/* ******************************************************************
 *                 FUNCION DE LECTURA DE COMANDOS
 * *****************************************************************/

lista_t* leer_linea() {

	char c;
	size_t tam = TAMANIO_INICIAL_BUFFER;
	int i;

	c = getchar();
	
	if (c == EOF) return NULL;

	lista_t *lista = lista_crear();
	
	if (!lista) return NULL;

	char* buffer = malloc(sizeof(char)*(tam));
	
	if (!buffer) return NULL;
	
	i = 0;
	
	while (c != ESPACIO && c != FIN_LINEA) {

		if (i + 1 == tam) {
			
			tam += tam;
			char* aux = realloc(buffer, tam * sizeof(char));
			if (!aux) return NULL;
			buffer = aux;
		}

		buffer[i] = c;
		c = getchar();
		i++;
	}

	buffer[i] = FIN_STRING;
	
	lista_insertar_ultimo(lista, buffer);
	
	if (c == FIN_LINEA) return lista;

	c = getchar();
	
	while (c != FIN_LINEA && c != EOF) {

		char* buffer1 = malloc(sizeof(char)*(tam));
		
		if (!buffer1) return NULL;
		
		i = 0;
		
		while (c != FIN_LINEA && c != COMA) {

			if (i + 1 == tam) {
				
				tam += tam;
				char* aux = realloc(buffer1, tam * sizeof(char));
				if (!aux) return NULL;
				buffer1 = aux;
			}

			buffer1[i] = c;
			c = getchar();
			i++;
		}

		buffer1[i] = FIN_STRING;
		
		lista_insertar_ultimo(lista, buffer1);
		
		if (c != FIN_LINEA) c = getchar();
	}

	return lista;
}

