#include <stdio.h>
#include <stdlib.h>

#include "parsing.h"
#include "lista.h"
#include "grafo.h"

#define TAMANIO_INICIAL 5

/* ******************************************************************
 *                  FUNCION PARA CARGAR EL GRAFO
 * *****************************************************************/

grafo_t* cargar_grafo() {

	char c;
	size_t tam = TAMANIO_INICIAL;
	int i;

	grafo_t* grafo = grafo_crear(NULL);
	
	if (!grafo) return NULL;

	c = getchar();
	
	if (c == EOF) return grafo;

	/* Lee la cantidad de vertices que va a tener el grafo */
	char* cantidad_vertices = malloc(sizeof(char)*(tam));
	
	if (!cantidad_vertices) return NULL;
	
	i = 0;
	
	while (c != FIN_LINEA) {
		
		cantidad_vertices[i] = c;
		c = getchar();
		i++;
	}
	
	cantidad_vertices[i] = FIN_STRING;
	printf("La cantidad de vertices es %s\n", cantidad_vertices);

	/* Inserta la cantidad de vertices especificada antes, con el nombre de cada vertice */
	int j = 0;
	while (j < atoi(cantidad_vertices)) {

		/* Saltea la parte de qué numero de vertice es */
		c = getchar();
		
		while (c != ESPACIO) {
			c = getchar();
		}

		c = getchar();
		int k = 0;
		char* vertice = malloc(sizeof(char)*(tam));
		
		if (!vertice) return NULL;
		
		while (c != FIN_LINEA) {
			
			vertice[k] = c;
			c = getchar();
			k++;
		}
		
		vertice[k] = FIN_STRING;

		grafo_insertar_vertice(grafo, vertice, NULL);
		free(vertice);
		j++;
	}

	free(cantidad_vertices);
	printf("Se cargaron todos los vertices\n");

	/* Lee la cantidad de aristas que va a tener el grafo */
	char* cantidad_aristas = malloc(sizeof(char)*(tam));
	
	if (!cantidad_aristas) return NULL;
	
	i = 0;
	c = getchar();
	
	while (c != FIN_LINEA) {
		
		cantidad_aristas[i] = c;
		c = getchar();
		i++;
	}

	cantidad_aristas[i] = FIN_STRING;
	printf("La cantidad de aristas es %s\n", cantidad_aristas);

	/* Inserta la cantidad de aristas especificada antes, uniendo los dos vertices que se leen */
	j = 0;
	
	while (j < atoi(cantidad_aristas)) {

		/* Se saltea qué numero de arista es */
		c = getchar();
		
		while (c != ESPACIO) {
			c = getchar();
		}

		lista_t* lista = lista_crear();
		
		if (!lista) return NULL;

		c = getchar();
		
		while (c != FIN_LINEA) {

			char* arista = malloc(sizeof(char)*(tam));
			
			if (!arista) return NULL;
			
			i = 0;
			
			while (c != ESPACIO && c != FIN_LINEA) {

				if (i + 1 == tam) {
					
					tam += tam;
					char* aux = realloc(arista, tam * sizeof(char));
					if (!aux) return NULL;
					arista = aux;
				}

				arista[i] = c;
				c = getchar();
				i++;
			}

			arista[i] = FIN_STRING;
			lista_insertar_ultimo(lista, arista);
			
			if (c != FIN_LINEA) {
				c = getchar();
			}
		}

		char* vertice1 = lista_borrar_primero(lista);
		char* vertice2 = lista_borrar_primero(lista);

		grafo_insertar_arista(grafo, vertice1, vertice2);

		free(vertice1); free(vertice2); lista_destruir(lista, NULL);

		j++;
	}
	
	free(cantidad_aristas);
	printf("Se cargaron todas las aristas\n");

	return grafo;
}

