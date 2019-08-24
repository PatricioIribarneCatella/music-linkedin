#include <stdlib.h>
#include <string.h>

#include "grafo.h"
#include "lista.h"
#include "vector.h"

#include "bfs.h"

// Reconstruye el camino BFS a partir
// de un arreglo con los padres de cada vertice.
static lista_t* reconstruir_camino(int* padre, char* vertice1, char* vertice2) {

	lista_t* lista = lista_crear();

	int* padre_aux1 = malloc(sizeof(int));
	*padre_aux1 = atoi(vertice2);
	lista_insertar_primero(lista, padre_aux1);
	
	int* padre_aux2 = malloc(sizeof(int));
	*padre_aux2 = padre[atoi(vertice2)];
	lista_insertar_primero(lista, padre_aux2);

	int i = padre[atoi(vertice2)];
	
	while (i != atoi(vertice1)) {

		int* padre_aux = malloc(sizeof(int));
		*padre_aux = padre[i];
		lista_insertar_primero(lista, padre_aux);
		i = padre[i];
	}

	return lista;
}

/* ***********************
 *         CAMINO
 * ***********************/

// Devuelve una lista con el camino entre dos vertices y sus extremos.
lista_t* calcular_camino(grafo_t* grafo, char* vertice1, char* vertice2) {

	int* padre = camino_BFS(grafo, vertice1, vertice2);
	
	if (!padre)
		return NULL;
	
	lista_t* camino = reconstruir_camino(padre, vertice1, vertice2);
	
	free(padre);
	
	return camino;
}

// Calcula todos los caminos minimos de
// todos los vertices entre todos los vertices.
vector_t* calcular_caminos(grafo_t* grafo) {

	vector_t* vector_aux = vector_crear(grafo_cantidad_vertices(grafo)*grafo_cantidad_vertices(grafo));
	lista_t* vertices = grafo_vertices(grafo);
	lista_iter_t* iter1 = lista_iter_crear(vertices);
	
	size_t pos = 0;

	while (!lista_iter_al_final(iter1)) {

		char* vertice = lista_iter_ver_actual(iter1);
		lista_iter_t* iter2 = lista_iter_crear(vertices);

		while (!lista_iter_al_final(iter2)) {

			if (strcmp(vertice, lista_iter_ver_actual(iter2)) != 0) {

				lista_t* camino_aux = calcular_camino(grafo, vertice, lista_iter_ver_actual(iter2));
				
				if (camino_aux) {
					vector_guardar(vector_aux, pos, camino_aux);
					pos++;
				}
			}

			lista_iter_avanzar(iter2);
		}

		lista_iter_destruir(iter2);
		lista_iter_avanzar(iter1);
	}

	lista_iter_destruir(iter1);
	lista_destruir(vertices, NULL);

	return vector_aux;
}
