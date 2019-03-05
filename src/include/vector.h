#ifndef VECTOR_DINAMICO_H
#define VECTOR_DINAMICO_H
#include <stdbool.h>

typedef struct vector vector_t;

vector_t* vector_crear(size_t tam);

void vector_destruir(vector_t *vector);

bool vector_redimensionar(vector_t *vector, size_t nuevo_tam);

void* vector_obtener_dato(vector_t* vector, size_t pos);

void* vector_obtener(const vector_t *vector, size_t pos);

bool vector_guardar(vector_t *vector, size_t pos, void* valor);

size_t vector_obtener_tamanio(vector_t *vector);

size_t vector_obtener_cantidad(vector_t* vector);

#endif // VECTOR_DINAMICO_H
