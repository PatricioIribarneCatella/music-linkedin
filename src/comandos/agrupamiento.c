#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"
#include "lista.h"

// Inicializa un arreglo con ceros para el clustering
static float* inicializar_clustering(grafo_t* grafo) {

	float* arreglo = malloc(sizeof(float)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = 0.0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con ceros para el hash
static int* inicializar_hash(grafo_t* grafo) {

	int* arreglo = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));

	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = 0;
	}

	return arreglo;
}

// Hashea vertices para luego contar las uniones entre ellos
static void hashear_vertices(char* vertice, lista_t* adyacentes, int* h) {

	lista_iter_t* iter = lista_iter_crear(adyacentes);

	while (!lista_iter_al_final(iter)) {

		char* v = lista_iter_ver_actual(iter);

		if (strcmp(v, vertice) != 0) {

			h[atoi(v)] = h[atoi(v)] + 1;
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
}

static float contar_cantidad_de_uniones(grafo_t* grafo, char* vertice,
			lista_t* adyacentes, lista_t* adyacentesA) {

	float cont = 0.0;

	int* h = inicializar_hash(grafo);

	hashear_vertices(vertice, adyacentesA, h);

	hashear_vertices(vertice, adyacentes, h);

	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {

		if (h[i] > 1) cont++;
	}

	free(h);

	return cont;
}

// Calcula cuantas uniones hay entre los vertices adyacentes a otro.
static float uniones_adyacentes(grafo_t* grafo, char* vertice, lista_t* adyacentes) {

	float contador = 0.0;

	lista_iter_t* iter = lista_iter_crear(adyacentes);
	
	while (!lista_iter_al_final(iter)) {

		char* verticeA = lista_iter_ver_actual(iter);

		lista_t* adyacentesA = grafo_adyacentes(grafo, verticeA);

		contador = contador + contar_cantidad_de_uniones(grafo, vertice, adyacentes, adyacentesA);

		lista_destruir(adyacentesA, NULL);

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);

	return contador;
}

// Calacula el coeficiente de agrupamiento para cada verice del grafo.
static float* agrupamiento_vertices(grafo_t* grafo) {

	float* clustering_v = inicializar_clustering(grafo);

	lista_t* vertices = grafo_vertices(grafo);
	lista_iter_t* iter = lista_iter_crear(vertices);

	while (!lista_iter_al_final(iter)) {

		char* vertice = lista_iter_ver_actual(iter);
		
		lista_t* adyacentes = grafo_adyacentes(grafo, vertice);

		if (grafo_cant_adyacentes(grafo, vertice) == 1) {
			
			clustering_v[atoi(vertice)] = 0.0;

		} else {
			
			float e = uniones_adyacentes(grafo, vertice, adyacentes);
			float k = grafo_cant_adyacentes(grafo, vertice)*(grafo_cant_adyacentes(grafo, vertice) - 1);

			clustering_v[atoi(vertice)] = e/k;
		}
		
		lista_destruir(adyacentes, NULL);
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(vertices, NULL);

	return clustering_v;
}

// Calcula el coeficiente de agrupamiento para todo el grafo
// a partir del de cada uno de los vertices.
static float agrupamiento_total(grafo_t* grafo, float* clustering_v) {

	float contador = 0;

	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		contador = contador + clustering_v[i];
	}

	free(clustering_v);
	
	return contador/grafo_cantidad_vertices(grafo);
}

// Calcula el coeficiente de agrupamiento/clustering del grafo.
static float calcular_agrupamiento(grafo_t* grafo) {

	float* clustering_v = agrupamiento_vertices(grafo);

	return agrupamiento_total(grafo, clustering_v);
}

/* *****************************
 *        AGRUPAMIENTO
 * ****************************/

void agrupamiento(grafo_t* grafo) {

	printf("El coeficiente de agrupamiento/clustering es %f\n",
			calcular_agrupamiento(grafo));
}

