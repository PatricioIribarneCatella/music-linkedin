#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct vector
{
	size_t tam;
	void* *datos;
	size_t cant_elem;
} vector_t;

vector_t* vector_crear(size_t tam)
{
	vector_t* vector = malloc(sizeof(vector_t));
	if (vector == NULL) return NULL;

	vector->datos = malloc(tam * sizeof(void*));
	if (vector->datos == NULL)
	{
	    free(vector);
	    return NULL;
	}
	vector->tam = tam;
	vector->cant_elem = 0;
	return vector;
}

void vector_destruir(vector_t *vector)
{
    free(vector->datos);
    free(vector);
}

bool vector_redimensionar(vector_t *vector, size_t tam_nuevo)
{
	void* datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(void*));
	if (tam_nuevo > 0 && datos_nuevo == NULL)
	{
	    return false;
	}
	vector->datos = datos_nuevo;
	vector->tam = tam_nuevo;
	return true;
}

void* vector_obtener_dato(vector_t *vector, size_t pos)
{
    if (pos >= vector->tam || pos<0)
    {
        return NULL;
    }
	void* valor = vector->datos[pos];
	(vector->cant_elem)--;
	return valor;
}

void* vector_obtener(const vector_t *vector, size_t pos)
{
    if (pos >= vector->tam || pos<0)
    {
        return NULL;
    }
	void* valor = vector->datos[pos];
	return valor;
}

bool vector_guardar(vector_t *vector, size_t pos, void* valor)
{
    if (pos >= vector->tam || pos<0)
    {
        return false;
    }
	vector->datos[pos] = valor;
	(vector->cant_elem)++;
	return true;
}

size_t vector_obtener_tamanio(vector_t *vector)
{
    return vector->tam;
}

size_t vector_obtener_cantidad(vector_t *vector)
{
    return vector->cant_elem;
}
