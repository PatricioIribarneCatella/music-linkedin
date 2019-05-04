#ifndef DESTRUIR_H
#define DESTRUIR_H

#include "lista.h"
#include "vector.h"

void destruir_linea(lista_t* lista);

// Destruye un vector con listas alamcenadas.
void destruir_vector(vector_t* vector);

// Destruye los siguientes arreglos utilizados en puntos de articulacion.
void destruir(int* padre, int* orden, int* mas_alto, int* contador);

// Destruye el parametro Rho.
void destruir_rho(lista_t* *rho, int tam);

#endif // DESTRUIR_H

