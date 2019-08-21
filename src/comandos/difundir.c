#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "grafo.h"
#include "lista.h"

#include "analizar.h"

// Defunde la informacion en el grafo.
static bool* difundir_informacion(grafo_t* grafo, lista_t* lista, int tam) {

	bool* informacion = inicializacion_difundir(lista, tam);

	bool* visitados = inicializar_visitados(grafo);

	lista_t* cola_BFS = lista_crear();

	while (!todos_visitados(visitados, grafo_cantidad_vertices(grafo))) {

		char* vertice = obtener_vertice(visitados, grafo_cantidad_vertices(grafo));

		visitados[atoi(vertice)] = true;

		analizar_componente_a_difundir(grafo, cola_BFS, vertice, visitados, informacion);
	}

	lista_destruir(cola_BFS, NULL);

	free(visitados);

	return informacion;
}

// Devuelve una lista con los vertices en los cuales se difundio la informacion.
static lista_t* difundir_grafo(grafo_t* grafo, lista_t* lista) {

	int tam = (int)grafo_cantidad_vertices(grafo);

	bool* informacion = difundir_informacion(grafo, lista, tam);

	lista_t* difundidos = analizar_informacion(informacion, tam);

	return difundidos;
}

// Verifica que los vertices a difundir pertenezcan al grafo.
static bool verificar_vertices_a_difundir(lista_t* lista, grafo_t* grafo) {

	lista_iter_t* iter = lista_iter_crear(lista);

	while (!lista_iter_al_final(iter)) {

		if (!grafo_pertence(grafo, lista_iter_ver_actual(iter))) {
			
			lista_iter_destruir(iter);
			
			return false;
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	
	return true;
}

/* *****************************
 *          DIFUNDIR
 * ****************************/

void difundir(grafo_t* grafo, lista_t* lista) {

	if (!lista_ver_primero(lista)) {
		
		printf("ERROR MUSICOS A DIFUNDIR INVALIDOS\n");
		return;
	}

	if (!verificar_vertices_a_difundir(lista, grafo)) {
		
		printf("ERROR ALGUN MUSICO A DIFUNDIR NO EXISTE\n");
		return;
	}

	lista_t* difundidos = difundir_grafo(grafo, lista);

	lista_iter_t* iter = lista_iter_crear(difundidos);
	printf("La cantidad de musicos a los cuales se difundió la informacion es %d\n",
			(int)lista_largo(difundidos));
	printf("Estos musicos son:\n");

	while (!lista_iter_al_final(iter)) {
		
		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(difundidos, free);
}

