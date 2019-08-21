#include <stdbool.h>

/* *****************************
 *            VISITAR
 * ****************************/

// Verfica si un vertice fue visitado o no.
bool visitado(bool* visitados, char* vertice) {

	return visitados[atoi(vertice)];
}

// Verifica si ya se visitaron todos los vertices del grafo.
bool todos_visitados(bool* visitados, size_t tam) {

	size_t i = 0;
	
	while (i < tam) {

		if (!visitados[i])
			return false;
		i++;
	}

	return true;
}

