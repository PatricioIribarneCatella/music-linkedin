#ifndef RECORRIDO_H
#define RECORRIDO_H

#include "grafo.h"
#include "lista.h"
#include "vector.h"

lista_t* calcular_camino(grafo_t* grafo, char* vertice1, char* vertice2);
vector_t* calcular_caminos(grafo_t* grafo);

#endif // RECORRIDO_H

