#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define TAM_INI 50
#define FACTOR 2

typedef struct _pila {
    size_t tam;
    size_t cant_elem;
    void* *datos;
} pila_t;

pila_t* pila_crear() {

    pila_t *pila = malloc (sizeof(pila_t));
    if (pila == NULL) return NULL;

    pila->datos = malloc (TAM_INI*sizeof(void*));

    if (pila->datos == NULL){
        free (pila);
        return NULL;
    }

    pila->tam = TAM_INI;
    pila->cant_elem = 0;
    return pila;
}

void pila_destruir(pila_t *pila){

    free (pila->datos);
    free (pila);
}

bool pila_esta_vacia(const pila_t *pila){

    if (pila->cant_elem == 0){
        return true;
    }
    return false;
}

bool redimensionar_pila (pila_t *pila, size_t tam_nuevo){
    void* datos_nuevo = realloc (pila->datos, tam_nuevo * sizeof(void*));

    if (datos_nuevo == NULL){
        return false;
    }

    pila->datos = datos_nuevo;
    pila->tam = tam_nuevo;
    return true;
}

bool redimensionar_p(pila_t *pila, int proporcion){

    if (proporcion == 33){
        return redimensionar_pila(pila, (pila->tam/2));
    }
    return false;
}

bool pila_apilar(pila_t *pila, void *valor){

    if (pila->cant_elem == pila->tam){
        if (redimensionar_pila(pila, FACTOR * pila->tam)){
            pila->datos[pila->cant_elem] = valor;
            pila->cant_elem = pila->cant_elem + 1;
            return true;
        }
        return false;
    }
    pila->datos[pila->cant_elem] = valor;
    pila->cant_elem = pila->cant_elem + 1;
    return true;
}

void* pila_ver_tope(const pila_t *pila){

    if (pila_esta_vacia(pila)){
        return NULL;
    }
    return pila->datos[pila->cant_elem-1];
}

void* pila_desapilar(pila_t *pila){

    int proporcion;
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    proporcion =((pila->cant_elem)/(pila->tam))*100;
    if (redimensionar_p(pila, proporcion)){
        pila->cant_elem = pila->cant_elem - 1;
        return pila->datos[pila->cant_elem];
    };
    pila->cant_elem = pila->cant_elem - 1;
    return pila->datos[pila->cant_elem];
}
