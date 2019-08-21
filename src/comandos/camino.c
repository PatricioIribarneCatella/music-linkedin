#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "lista.h"

#include "bfs.h"

/* **************
 *    CAMINO
 * **************/

void camino(grafo_t* grafo, lista_t* lista) {

	char* vertice1 = lista_borrar_primero(lista);
	
	if (!vertice1) {
		
		printf("ERROR COMANDO INVALIDO\n");
		return;
	}
	
	if (!grafo_pertence(grafo, vertice1)) {
		
		printf("MUSICO INEXISTENTE\n");
		free(vertice1);
		
		return;
	}
	
	char* vertice2 = lista_borrar_primero(lista);
	
	if (!vertice2) {
		
		free(vertice1);
		printf("ERROR COMANDO INVALIDO\n");
		
		return;
	}
	
	if (!grafo_pertence(grafo, vertice2)) {
		
		printf("MUSICO INEXISTENTE\n");
		free(vertice2);
		free(vertice1);
		
		return;
	}

	lista_t* recorrido = calcular_camino(grafo, vertice1, vertice2);
	
	if (!recorrido) {
		
		printf("EL CAMINO ENTRE LOS MUSICOS NO EXISTE\n");
		return;
	}

	lista_iter_t* iter = lista_iter_crear(recorrido);
	printf("El camino entre %s y %s es:\n", vertice1, vertice2);

	while (!lista_iter_al_final(iter)) {
		
		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	lista_destruir(recorrido, free);
	free(vertice1);
	free(vertice2);
}

