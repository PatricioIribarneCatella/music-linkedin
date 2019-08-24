#ifndef CENTRAL_H
#define CENTRAL_H

#include <stddef.h>

typedef struct central {
	int* vertice;
	int* apariciones;
} central_t;

void destruir_central(void* dato);
central_t* crear_central(int vertice, int apariciones);

#endif // CENTRAL_H

