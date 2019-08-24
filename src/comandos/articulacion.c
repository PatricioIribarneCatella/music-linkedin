#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "grafo.h"
#include "lista.h"

#include "analizar.h"

// Devuelve una lista con los puntos de articulación
// teniendo en cuenta un arreglo de bool.
static lista_t* extraer_puntos(bool* puntos, int tam) {

	lista_t* lista = lista_crear();

	for (int i = 0; i < tam; i++) {

		if (puntos[i]) {

			int* j = malloc(sizeof(int));
			*j = i;
			lista_insertar_primero(lista, j);
		}
	}

	free(puntos);
	
	return lista;
}

// Calcula los puntos de articulacion del grafo.
static lista_t* calcular_puntos_articulacion(grafo_t* grafo) {

	bool* puntos = analizar_puntos(grafo);

	return extraer_puntos(puntos, (int)grafo_cantidad_vertices(grafo));
}

/* *****************************
 *    PUNTOS DE ARTICULACION
 * ****************************/

void articulacion(grafo_t* grafo) {

	lista_t* puntos = calcular_puntos_articulacion(grafo);

	if (lista_esta_vacia(puntos)) {

		printf("El grafo no contiene puntos de articulacion\n");
		lista_destruir(puntos, NULL);
		
		return;
	}

	lista_iter_t* iter = lista_iter_crear(puntos);
	printf("Los puntos de articulacion son:\n");

	while (!lista_iter_al_final(iter)) {

		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(puntos, free);
}

