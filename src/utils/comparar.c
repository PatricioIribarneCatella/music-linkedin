#include "central.h"

/* *****************************
 *         COMPARACION
 * ****************************/

// Función de comparación 1
// (heap de mínimos).
int cmp1(const void *a, const void *b) {

	int* c = (int*)a;
	int* d = (int*)b;

	if (*c < *d)
		return 1;
	else if (*c > *d)
		return -1;
	else
		return 0;
}

// Función de comparacioón 2
// (heap de máximos).
int cmp2(const void *a, const void *b) {

	int c = *(int*)a;
	int d = *(int*)b;

	if (c > d)
		return 1;
	else if (c < d)
		return -1;
	else
		return 0;
}

// Función de comparación 3
// (heap de máximos guardando estructura central).
int cmp3(const void *a, const void *b) {

	central_t* c = (central_t*)a;
	central_t* d = (central_t*)b;

	if (*c->apariciones > *d->apariciones)
		return 1;
	else if (*c->apariciones < *d->apariciones)
		return -1;
	else
		return 0;
}

