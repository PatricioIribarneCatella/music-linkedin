#include <stdlib.h>

#include "lista.h"
#include "vector.h"

/* *****************************
 *    DESTRUCCION DE DATOS
 * ****************************/

// Destruye la linea leída por entrada estandar.
void destruir_linea(lista_t* lista) {

	lista_destruir(lista, free);
}

// Destruye un vector con listas alamcenadas.
void destruir_vector(vector_t* vector) {

	for (size_t i = 0; i < vector_obtener_cantidad(vector); i++) {
		
		lista_t* lista = vector_obtener(vector, i);
		lista_destruir(lista, free);
	}

	vector_destruir(vector);
}

// Destruye los siguientes arreglos utilizados en puntos de articulacion.
void destruir(int* padre, int* orden, int* mas_alto, int* contador) {

	free(padre);
	free(orden);
	free(mas_alto);
	free(contador);
}

// Destruye el parametro Rho.
void destruir_rho(lista_t* *rho, int tam) {

	for (int i = 0; i < tam; i++) {
		lista_destruir(rho[i], NULL);
	}
	
	free(rho);
}

