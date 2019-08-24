#include <stdlib.h>
#include <stdbool.h>

#include "grafo.h"
#include "lista.h"

#include "visitar.h"

/* **********************************************
 *    DFS (articulación) Algoritmo de Tarjam
 * *********************************************/

// Devuelve el valor minimo entre dos enteros.
int minimo(int a, int b) {

	if (a < b) return a;
	return b;
}

void DFS_articulacion(grafo_t* grafo,
			char* vertice,
			int* padre,
			bool* visitados,
			int* orden,
			int* mas_alto,
			int* contador) {

	visitados[atoi(vertice)] = true;
	(*contador)++;
	orden[atoi(vertice)] = mas_alto[atoi(vertice)] = *contador;
	lista_t* adyacentes = grafo_adyacentes(grafo, vertice);
	lista_iter_t* iter = lista_iter_crear(adyacentes);

	while (!lista_iter_al_final(iter)) {

		char* w = lista_iter_ver_actual(iter);

		if (!visitado(visitados, w)) {

			padre[atoi(w)] = atoi(vertice);
			DFS_articulacion(grafo, w, padre, visitados, orden, mas_alto, contador);
			mas_alto[atoi(vertice)] = minimo(mas_alto[atoi(vertice)], mas_alto[atoi(w)]);

		} else {

			if (padre[atoi(vertice)] != atoi(w))
				mas_alto[atoi(vertice)] = minimo(mas_alto[atoi(vertice)], orden[atoi(w)]);
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(adyacentes, NULL);
}

