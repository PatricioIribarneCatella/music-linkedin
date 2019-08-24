#ifndef PARSING_H
#define PARSING_H

#include "lista.h"
#include "grafo.h"

#define FIN_LINEA '\n'
#define FIN_STRING '\0'
#define ESPACIO ' '
#define COMA ','

grafo_t* cargar_grafo();
lista_t* leer_linea();

#endif // PARSING_H
