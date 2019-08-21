#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "lista.h"
#include "vector.h"

#include "bfs.h"

// Devuelve una lista con los vertices que componen
// el camino maximo de todos los caminos minimos.
lista_t* calcular_diametro(grafo_t* grafo) {

	vector_t* caminos = calcular_caminos(grafo);
	lista_t* camino_max = camino_diametro(caminos);
	
	destruir_vector(caminos);
	
	return camino_max;
}

// A partir de todos los caminos minimos, busca el que posee mayor largo.
lista_t* camino_diametro(vector_t* caminos) {

	int pos_maximo = camino_maximo(caminos);
	
	lista_t* maximo = vector_obtener(caminos, pos_maximo);
	
	lista_t* lista = lista_copiar(maximo);
	
	return lista;
}

// Devuelve la posicion donde se encuentra el camino mas largo.
int camino_maximo(vector_t* caminos) {

	int pos = 0; int i = 0; size_t largo;

	largo = lista_largo((lista_t*)vector_obtener(caminos, i));
	i++;

	while (i < vector_obtener_cantidad(caminos)) {

		if (lista_largo((lista_t*)vector_obtener(caminos, i)) > largo) {

			largo = lista_largo((lista_t*)vector_obtener(caminos, i));
			pos = i;
		}
		
		i++;
	}

	return pos;
}

/* *****************************
 *          DIAMETRO
 * ****************************/

void diametro(grafo_t* grafo) {

	lista_t* camino_diametro = calcular_diametro(grafo);
	lista_iter_t* iter = lista_iter_crear(camino_diametro);

	printf("El diametro es %d\n", (int)lista_largo(camino_diametro)-1);
	printf("Los músicos que componen el camino son:\n");

	while (!lista_iter_al_final(iter)) {
		
		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	lista_destruir(camino_diametro, free);
}


