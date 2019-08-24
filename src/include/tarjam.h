#ifndef TARJAM_H
#define TARJAM_H

#include <stdbool.h>

#include "grafo.h"

void DFS_articulacion(grafo_t* grafo,
			char* vertice,
			int* padre,
			bool* visitados,
			int* orden,
			int* mas_alto,
			int* contador); 

#endif // TARJAM_H

