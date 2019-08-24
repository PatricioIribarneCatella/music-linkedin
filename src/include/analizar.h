#ifndef ANALIZAR_H
#define ANALIZAR_H

#include <stdbool.h>

#include "grafo.h"
#include "lista.h"

// utils
char* obtener_vertice(bool* visitados, size_t tam);

// Articulación
bool* analizar_puntos(grafo_t* grafo);

// Subgrafos
int analizar_vertice(grafo_t* grafo, lista_t* cola_BFS, bool* visitados);

// Difundir
void analizar_componente_a_difundir(grafo_t* grafo,
				    lista_t* cola_BFS,
				    char* vertice,
				    bool* visitados,
				    bool* informacion);
lista_t* analizar_informacion(bool* informacion, int tam);

#endif // ANALIZAR_H

