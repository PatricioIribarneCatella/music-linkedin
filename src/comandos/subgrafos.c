#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "lista.h"
#include "heap.h"

#include "analizar.h"
#include "comparar.h"
#include "visitar.h"
#include "inicializar.h"

// Calcula las componentes conexas del grafo.
static heap_t* calcular_subgrupos(grafo_t* grafo) {

	lista_t* cola_BFS = lista_crear();
	bool* visitados = inicializar_visitados(grafo);
	heap_t* componentes = heap_crear(cmp2, grafo_cantidad_vertices(grafo));

	while (!todos_visitados(visitados, grafo_cantidad_vertices(grafo))) {

		char* vertice = obtener_vertice(visitados, grafo_cantidad_vertices(grafo));
		lista_t* adyacentes = grafo_adyacentes(grafo, vertice);
		lista_iter_t* iter = lista_iter_crear(adyacentes);

		visitados[atoi(vertice)] = true;
		
		while (!lista_iter_al_final(iter)) {
			
			lista_insertar_primero(cola_BFS, lista_iter_ver_actual(iter));
			lista_iter_avanzar(iter);
		}
		
		lista_iter_destruir(iter);
		lista_destruir(adyacentes, NULL);

		int vertices_analizados = analizar_vertice(grafo, cola_BFS, visitados);
		int* componente_conexa = malloc(sizeof(int));
		*componente_conexa = vertices_analizados;
		heap_encolar(componentes, componente_conexa);
	}

	lista_destruir(cola_BFS, NULL);
	free(visitados);
	
	return componentes;
}

/* *****************************
 *          SUBGRUPOS
 * ****************************/

void subgrupos(grafo_t* grafo) {

	heap_t* grupos = calcular_subgrupos(grafo);
	
	printf("La cantidad de subgrupos/componentes conexas es: %d\n",
			(int)heap_cantidad(grupos));
	
	printf("Cantidad de musicos por subgrupo\n");

	int i = 1;
	
	while (!heap_esta_vacio(grupos)) {
		printf("Subgrupo %d\n", i);
		int* subgrupo = heap_desencolar(grupos);
		printf("%d\n", *subgrupo);
		free(subgrupo);
		i++;
	}

	heap_destruir(grupos, NULL);
}


