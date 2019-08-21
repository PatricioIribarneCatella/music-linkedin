#ifndef INICIALIZAR_H
#define INICIALIZAR_H

#include <stdbool.h>

#include "grafo.h"
#include "lista.h"

bool* inicializar_visitados(grafo_t* grafo);
int* inicializar_padre(grafo_t* grafo);
int* inicializar_orden(grafo_t* grafo);
int* inicializar_mas_alto(grafo_t* grafo);
bool* inicializar_articulacion(grafo_t* grafo);
bool* inicializacion_difundir(lista_t* lista, int tam);
int* inicializar_centralidad(grafo_t* grafo);
int* inicializar_sigma(grafo_t* grafo);
int* inicializar_distancia(grafo_t* grafo);
int* inicializar_delta(grafo_t* grafo);
lista_t* *inicializar_rho(grafo_t* grafo);

#endif // INICIALIZAR_H

