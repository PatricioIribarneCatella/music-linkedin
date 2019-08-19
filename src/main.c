#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "vector.h"
#include "lista.h"
#include "pila.h"
#include "heap.h"
#include "hash.h"
#include "grafo.h"

#include "destruir.h"
#include "parsing.h"
#include "comandos.h"

/* ******************************************************************
 *                         Music-LinkeIn
 * *****************************************************************/

void linkedin() {

	grafo_t* grafo = cargar_grafo();
	
	if (!grafo) {
		
		printf("ERROR INESPERADO\n");
		return;
	}

	if (grafo_esta_vacio(grafo)) {
		
		grafo_destruir(grafo);
		return;
	}

	printf("GRAFO CARGADO\n\n");

	lista_t* linea = leer_linea();
	
	while (linea != NULL) {

		procesar_linea(linea, grafo);
		destruir_linea(linea);
		linea = leer_linea();
	}

	grafo_destruir(grafo);
}

int main(void) {

	linkedin();

	return 0;
}

