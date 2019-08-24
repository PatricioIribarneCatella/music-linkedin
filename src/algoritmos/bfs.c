#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "grafo.h"
#include "vector.h"
#include "lista.h"

#include "inicializar.h"
#include "visitar.h"

/* ***********************
 *          BFS
 * ***********************/

// Realiza un recorrido BFS del grafo para encontrar
// el camino mínimo entre dos vertices.
int* camino_BFS(grafo_t* grafo, char* vertice1, char* vertice2) {

	lista_t* cola_BFS = lista_crear();
	bool* visitados = inicializar_visitados(grafo);
	lista_t* adyacentes = grafo_adyacentes(grafo, vertice1);
	visitados[atoi(vertice1)] = true;

	lista_iter_t* iter = lista_iter_crear(adyacentes);
	int* padre = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		padre[i] = grafo_cantidad_vertices(grafo);
	}

	while (!lista_iter_al_final(iter)) {

		lista_insertar_primero(cola_BFS, lista_iter_ver_actual(iter));
		
		if (padre[atoi((char*)lista_iter_ver_actual(iter))] == grafo_cantidad_vertices(grafo))
			padre[atoi((char*)lista_iter_ver_actual(iter))] = atoi(vertice1);
		
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	lista_destruir(adyacentes, NULL);

	bool encontrado = false; char* vertice;
	
	while (!lista_esta_vacia(cola_BFS) && !encontrado) {

		vertice = lista_borrar_primero(cola_BFS);

		if (!visitado(visitados, vertice)) {

			if (strcmp(vertice, vertice2) != 0) {

				lista_t* adyacentes_aux = grafo_adyacentes(grafo, vertice);
				lista_iter_t* iter_aux = lista_iter_crear(adyacentes_aux);

				while (!lista_iter_al_final(iter_aux)) {

					lista_insertar_ultimo(cola_BFS, lista_iter_ver_actual(iter_aux));
					
					if (padre[atoi((char*)lista_iter_ver_actual(iter_aux))] == grafo_cantidad_vertices(grafo))
						padre[atoi((char*)lista_iter_ver_actual(iter_aux))] = atoi(vertice);
					
					lista_iter_avanzar(iter_aux);
				}

				lista_iter_destruir(iter_aux);
				lista_destruir(adyacentes_aux, NULL);
				
				visitados[atoi(vertice)] = true;

			} else {
				
				encontrado = true;
			}
		}
	}

	if (lista_esta_vacia(cola_BFS)) {

		free(padre);
		free(visitados);
		lista_destruir(cola_BFS, NULL);

		return NULL;
	}

	lista_destruir(cola_BFS, NULL);
	free(visitados);
	
	return padre;
}


