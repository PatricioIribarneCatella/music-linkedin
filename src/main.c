#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "vector.h"
#include "lista.h"
#include "pila.h"
#include "heap.h"
#include "hash.h"
#include "grafo.h"

#define RECOMENDAR "recomendar"
#define DIFUNDIR "difundir"
#define CENTRALIDAD "centralidad"
#define DIAMETRO "diametro"
#define CAMINO "camino"
#define SUBGRUPOS "subgrupos"
#define ARTICULACION "articulacion"
#define AGRUPAMIENTO "agrupamiento"

#define FIN_LINEA '\n'
#define FIN_STRING '\0'
#define ESPACIO ' '
#define COMA ','

#define TAMANIO_INICIAL_BUFFER 10
#define TAMANIO_INICIAL 5

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct central {
	int* vertice;
	int* apariciones;
} central_t;

/* ******************************************************************
 *                 FUNCION DE LECTURA DE COMANDOS
 * *****************************************************************/

lista_t* leer_linea() {

	char c;
	size_t tam = TAMANIO_INICIAL_BUFFER;
	int i;

	c = getchar();
	
	if (c == EOF) return NULL;

	lista_t *lista = lista_crear();
	
	if (!lista) return NULL;

	char* buffer = malloc(sizeof(char)*(tam));
	
	if (!buffer) return NULL;
	
	i = 0;
	
	while (c != ESPACIO && c != FIN_LINEA) {

		if (i + 1 == tam) {
			
			tam += tam;
			char* aux = realloc(buffer, tam * sizeof(char));
			if (!aux) return NULL;
			buffer = aux;
		}

		buffer[i] = c;
		c = getchar();
		i++;
	}

	buffer[i] = FIN_STRING;
	
	lista_insertar_ultimo(lista, buffer);
	
	if (c == FIN_LINEA) return lista;

	c = getchar();
	
	while (c != FIN_LINEA && c != EOF) {

		char* buffer1 = malloc(sizeof(char)*(tam));
		
		if (!buffer1) return NULL;
		
		i = 0;
		
		while (c != FIN_LINEA && c != COMA) {

			if (i + 1 == tam) {
				
				tam += tam;
				char* aux = realloc(buffer1, tam * sizeof(char));
				if (!aux) return NULL;
				buffer1 = aux;
			}

			buffer1[i] = c;
			c = getchar();
			i++;
		}

		buffer1[i] = FIN_STRING;
		
		lista_insertar_ultimo(lista, buffer1);
		
		if (c != FIN_LINEA) c = getchar();
	}

	return lista;
}

/* ******************************************************************
 *                  FUNCION PARA CARGAR EL GRAFO
 * *****************************************************************/

grafo_t* cargar_grafo() {

	char c;
	size_t tam = TAMANIO_INICIAL;
	int i;

	grafo_t* grafo = grafo_crear(NULL);
	
	if (!grafo) return NULL;

	c = getchar();
	
	if (c == EOF) return grafo;

	/* Lee la cantidad de vertices que va a tener el grafo */
	char* cantidad_vertices = malloc(sizeof(char)*(tam));
	
	if (!cantidad_vertices) return NULL;
	
	i = 0;
	
	while (c != FIN_LINEA) {
		
		cantidad_vertices[i] = c;
		c = getchar();
		i++;
	}
	
	cantidad_vertices[i] = FIN_STRING;
	printf("La cantidad de vertices es %s\n", cantidad_vertices);

	/* Inserta la cantidad de vertices especificada antes, con el nombre de cada vertice */
	int j = 0;
	while (j < atoi(cantidad_vertices)) {

		/* Saltea la parte de qué numero de vertice es */
		c = getchar();
		
		while (c != ESPACIO) {
			c = getchar();
		}

		c = getchar();
		int k = 0;
		char* vertice = malloc(sizeof(char)*(tam));
		
		if (!vertice) return NULL;
		
		while (c != FIN_LINEA) {
			
			vertice[k] = c;
			c = getchar();
			k++;
		}
		
		vertice[k] = FIN_STRING;

		grafo_insertar_vertice(grafo, vertice, NULL);
		free(vertice);
		j++;
	}

	free(cantidad_vertices);
	printf("Se cargaron todos los vertices\n");

	/* Lee la cantidad de aristas que va a tener el grafo */
	char* cantidad_aristas = malloc(sizeof(char)*(tam));
	
	if (!cantidad_aristas) return NULL;
	
	i = 0;
	c = getchar();
	
	while (c != FIN_LINEA) {
		
		cantidad_aristas[i] = c;
		c = getchar();
		i++;
	}

	cantidad_aristas[i] = FIN_STRING;
	printf("La cantidad de aristas es %s\n", cantidad_aristas);

	/* Inserta la cantidad de aristas especificada antes, uniendo los dos vertices que se leen */
	j = 0;
	
	while (j < atoi(cantidad_aristas)) {

		/* Se saltea qué numero de arista es */
		c = getchar();
		
		while (c != ESPACIO) {
			c = getchar();
		}

		lista_t* lista = lista_crear();
		
		if (!lista) return NULL;

		c = getchar();
		
		while (c != FIN_LINEA) {

			char* arista = malloc(sizeof(char)*(tam));
			
			if (!arista) return NULL;
			
			i = 0;
			
			while (c != ESPACIO && c != FIN_LINEA) {

				if (i + 1 == tam) {
					
					tam += tam;
					char* aux = realloc(arista, tam * sizeof(char));
					if (!aux) return NULL;
					arista = aux;
				}

				arista[i] = c;
				c = getchar();
				i++;
			}

			arista[i] = FIN_STRING;
			lista_insertar_ultimo(lista, arista);
			
			if (c != FIN_LINEA) {
				c = getchar();
			}
		}

		char* vertice1 = lista_borrar_primero(lista);
		char* vertice2 = lista_borrar_primero(lista);

		grafo_insertar_arista(grafo, vertice1, vertice2);

		free(vertice1); free(vertice2); lista_destruir(lista, NULL);

		j++;
	}
	
	free(cantidad_aristas);
	printf("Se cargaron todas las aristas\n");

	return grafo;
}

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

/* *****************************
 *            ITOA
 * ****************************/

// Convierte un entero a string. Fuente: http://newbiegeek.com/convertir-un-entero-a-cadena-de-caracteres-en-c/
char* itoa(int val, int base) {

	char* zero = "0";
	if (val == 0) return zero;

	static char buf[32] = {0};
	int i = 30;
	
	for (; val && i ; --i, val /= base) {
		buf[i] = "0123456789abcdef"[val % base];
	}
	
	return &buf[i+1];
}

/* *****************************
 *       FUNCIONES LISTA
 * ****************************/

// Une dos listas en una.
lista_t* union_lista(lista_t* lista1, lista_t* lista_2) {

	lista_iter_t* iter = lista_iter_crear(lista_2);
	
	while (!lista_iter_al_final(iter)) {

		lista_insertar_ultimo(lista1, lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	
	return lista1;
}

// Copia el valor de un puntero a entero a otro.
int* copiar_dato(int* dato) {

	int* dato_copiado = malloc(sizeof(int));
	*dato_copiado = *dato;
	
	return dato_copiado;
}

// Copia una lista.
lista_t* copiar_lista(lista_t* lista) {

	lista_t* lista_aux = lista_crear();
	lista_iter_t* iter = lista_iter_crear(lista);

	while (!lista_iter_al_final(iter)) {

		int* copia = copiar_dato(lista_iter_ver_actual(iter));
		lista_insertar_ultimo(lista_aux, copia);
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	
	return lista_aux;
}

/* *****************************
 *            VISITAR
 * ****************************/

// Verfica si un vertice fue visitado o no.
bool visitado(bool* visitados, char* vertice) {

	return visitados[atoi(vertice)];
}

// Verifica si ya se visitaron todos los vertices del grafo.
bool todos_visitados(bool* visitados, size_t tam) {

	size_t i = 0;
	
	while (i < tam) {

		if (!visitados[i]) return false;
		i++;
	}

	return true;
}

/* *****************************
 *    DESTRUCCION DE DATOS
 * ****************************/

// Destruye la linea leída por entrada estandar.
void destruir_linea(lista_t* lista) {

	lista_destruir(lista, free);
}

// Destruye un vector con listas alamcenadas.
void destruir_vector(vector_t* vector) {

	for (size_t i = 0; i < vector_obtener_cantidad(vector); i++) {
		
		lista_t* lista = vector_obtener(vector, i);
		lista_destruir(lista, free);
	}

	vector_destruir(vector);
}

// Destruye la estructura de central.
void destruir_central(void* dato) {

	central_t* central = (central_t*)dato;

	free(central->vertice);
	free(central->apariciones);
	free(central);
}

// Destruye los siguientes arreglos utilizados en puntos de articulacion.
void destruir(int* padre, int* orden, int* mas_alto, int* contador) {

	free(padre);
	free(orden);
	free(mas_alto);
	free(contador);
}

// Destruye el parametro Rho.
void destruir_rho(lista_t* *rho, int tam) {

	for (int i = 0; i < tam; i++) {
		lista_destruir(rho[i], NULL);
	}
	
	free(rho);
}

/* *****************************
 *         COMPARACION
 * ****************************/

// Función de comparación 1 (heap de minimos).
int cmp1(const void *a, const void *b) {

	int* c = (int*)a;
	int* d = (int*)b;

	if (*c < *d) {
		return 1;
	}else if (*c > *d) {
		return -1;
	}else{
		return 0;
	}
}

// Funcion de comparacion 2 (heap de maximos).
int cmp2(const void *a, const void *b) {

	int c = *(int*)a;
	int d = *(int*)b;

	if (c > d) {
		return 1;
	}else if (c < d) {
		return -1;
	}else{
		return 0;
	}
}

// Funcion de comparacion 3 (heap de maximos guardando estructura central).
int cmp3(const void *a, const void *b) {

	central_t* c = (central_t*)a;
	central_t* d = (central_t*)b;

	if (*c->apariciones > *d->apariciones) {
		return 1;
	}else if (*c->apariciones < *d->apariciones) {
		return -1;
	}else{
		return 0;
	}
}

/* *****************************
 *       INICIALIZACION
 * ****************************/

// Inicializa un arreglo con false para los visitados.
bool* inicializar_visitados(grafo_t* grafo) {

	bool* visita = malloc(sizeof(bool)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		visita[i] = false;
	}
	
	return visita;
}

// Inicializa un arreglo con la cantidad de vertices en cada posicion para los padres.
int* inicializar_padre(grafo_t* grafo) {

	int* padre = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		padre[i] = grafo_cantidad_vertices(grafo);
	}
	
	return padre;
}

// Inicializa un arreglo con ceros para el orden.
int* inicializar_orden(grafo_t* grafo) {

	int* orden = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		orden[i] = 0;
	}
	
	return orden;
}

// Inicializa un arreglo con ceros para mas_alto.
int* inicializar_mas_alto(grafo_t* grafo) {

	int* mas_alto = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		mas_alto[i] = 0;
	}
	
	return mas_alto;
}

// Inicializa un arreglo con false para los puntos de articulacion.
bool* inicializar_articulacion(grafo_t* grafo) {

	bool* ptos_articulacion = malloc(sizeof(bool)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		ptos_articulacion[i] = false;
	}
	
	return ptos_articulacion;
}

// Inicializa el contador en cero y registra los vertices donde se va plantar la informacion a difundir.
bool* inicializacion_difundir(lista_t* lista, int tam) {

	bool* arreglo = malloc(sizeof(bool)*tam);
	
	for (int i = 0; i < tam; i++) {
		arreglo[i] = false;
	}

	lista_iter_t* iter = lista_iter_crear(lista);
	
	while (!lista_iter_al_final(iter)) {
		
		arreglo[atoi((char*)lista_iter_ver_actual(iter))] = true;
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);

	return arreglo;
}

// Inicializa un arreglo con 0 para la centralidad.
int* inicializar_centralidad(grafo_t* grafo) {

	int* arreglo = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = 0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con 0 para el parametro Sigma.
int* inicializar_sigma(grafo_t* grafo) {

	int* arreglo = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = 0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con -1 para la distancia.
int* inicializar_distancia(grafo_t* grafo) {

	int* arreglo = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = -1;
	}
	
	return arreglo;
}

// Inicializa un arreglo con 0 para el parametro Delta.
int* inicializar_delta(grafo_t* grafo) {

	int* arreglo = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = 0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con listas vacias para el parametro Rho.
lista_t* *inicializar_rho(grafo_t* grafo) {

	lista_t* *arreglo = malloc(sizeof(lista_t*)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = lista_crear();
	}
	
	return arreglo;
}

/* *****************************
 *         CAMINO BFS
 * ****************************/

// Realiza un recorrido BFS del grafo para encontrar el camino mínimo entre dos vertices.
int* camino_BFS(grafo_t* grafo, char* vertice1, char* vertice2) {

	lista_t* cola_BFS = lista_crear();
	bool* visitados = inicializar_visitados(grafo);
	lista_t* adyacentes = grafo_adyacentes(grafo, vertice1);
	visitados[atoi(vertice1)] = true;

	lista_iter_t* iter = lista_iter_crear(adyacentes);
	int* padre = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		padre[i] = grafo_cantidad_vertices(grafo);
	}

	while (!lista_iter_al_final(iter)) {

		lista_insertar_primero(cola_BFS, lista_iter_ver_actual(iter));
		
		if (padre[atoi((char*)lista_iter_ver_actual(iter))] == grafo_cantidad_vertices(grafo)) padre[atoi((char*)lista_iter_ver_actual(iter))] = atoi(vertice1);
		
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	lista_destruir(adyacentes, NULL);

	bool encontrado = false; char* vertice;
	
	while (!lista_esta_vacia(cola_BFS) && !encontrado) {

		vertice = lista_borrar_primero(cola_BFS);

		if (!visitado(visitados, vertice)) {

			if (strcmp(vertice, vertice2) != 0) {

				lista_t* adyacentes_aux = grafo_adyacentes(grafo, vertice);
				lista_iter_t* iter_aux = lista_iter_crear(adyacentes_aux);

				while (!lista_iter_al_final(iter_aux)) {

					lista_insertar_ultimo(cola_BFS, lista_iter_ver_actual(iter_aux));
					
					if (padre[atoi((char*)lista_iter_ver_actual(iter_aux))] == grafo_cantidad_vertices(grafo)) padre[atoi((char*)lista_iter_ver_actual(iter_aux))] = atoi(vertice);
					
					lista_iter_avanzar(iter_aux);
				}

				lista_iter_destruir(iter_aux);
				lista_destruir(adyacentes_aux, NULL);
				
				visitados[atoi(vertice)] = true;

			} else {
				
				encontrado = true;
			}
		}
	}

	if (lista_esta_vacia(cola_BFS)) {

		free(padre);
		free(visitados);
		lista_destruir(cola_BFS, NULL);

		return NULL;
	}

	lista_destruir(cola_BFS, NULL);
	free(visitados);
	
	return padre;
}

// Reconstruye el camino BFS a partir de un arreglo con los padres de cada vertice.
lista_t* reconstruir_camino(int* padre, char* vertice1, char* vertice2) {

	lista_t* lista = lista_crear();

	int* padre_aux1 = malloc(sizeof(int));
	*padre_aux1 = atoi(vertice2);
	lista_insertar_primero(lista, padre_aux1);
	int* padre_aux2 = malloc(sizeof(int));
	*padre_aux2 = padre[atoi(vertice2)];
	lista_insertar_primero(lista, padre_aux2);

	int i = padre[atoi(vertice2)];
	
	while (i != atoi(vertice1)) {

		int* padre_aux = malloc(sizeof(int));
		*padre_aux = padre[i];
		lista_insertar_primero(lista, padre_aux);
		i = padre[i];
	}

	return lista;
}

// Devuelve una lista con el camino entre dos vertices y sus extremos.
lista_t* calcular_camino(grafo_t* grafo, char* vertice1, char* vertice2) {

	int* padre = camino_BFS(grafo, vertice1, vertice2);
	
	if (!padre) return NULL;
	
	lista_t* camino = reconstruir_camino(padre, vertice1, vertice2);
	free(padre);
	
	return camino;
}

// Calcula todos los caminos minimos de todos los vertices entre todos los vertices.
vector_t* calcular_caminos(grafo_t* grafo) {

	vector_t* vector_aux = vector_crear(grafo_cantidad_vertices(grafo)*grafo_cantidad_vertices(grafo));
	lista_t* vertices = grafo_vertices(grafo);
	lista_iter_t* iter1 = lista_iter_crear(vertices);
	size_t pos = 0;

	while (!lista_iter_al_final(iter1)) {

		char* vertice = lista_iter_ver_actual(iter1);
		lista_iter_t* iter2 = lista_iter_crear(vertices);

		while (!lista_iter_al_final(iter2)) {

			if (strcmp(vertice, lista_iter_ver_actual(iter2)) != 0) {

				lista_t* camino_aux = calcular_camino(grafo, vertice, lista_iter_ver_actual(iter2));
				
				if (camino_aux != NULL) {
					vector_guardar(vector_aux, pos, camino_aux);
					pos++;
				}
			}

			lista_iter_avanzar(iter2);
		}

		lista_iter_destruir(iter2);
		lista_iter_avanzar(iter1);
	}

	lista_iter_destruir(iter1);
	lista_destruir(vertices, NULL);

	return vector_aux;
}

/* **********************************************
 *    DFS (articulación) Algoritmo de Tarjam
 * *********************************************/

// Devuelve el valor minimo entre dos enteros.
int minimo(int a, int b) {

	if (a < b) return a;
	return b;
}

void DFS_articulacion(grafo_t* grafo, char* vertice, int* padre, bool* visitados, int* orden, int* mas_alto, int* contador) {

	visitados[atoi(vertice)] = true;
	(*contador)++;
	orden[atoi(vertice)] = mas_alto[atoi(vertice)] = *contador;
	lista_t* adyacentes = grafo_adyacentes(grafo, vertice);
	lista_iter_t* iter = lista_iter_crear(adyacentes);

	while (!lista_iter_al_final(iter)) {

		char* w = lista_iter_ver_actual(iter);

		if (!visitado(visitados, w)) {

			padre[atoi(w)] = atoi(vertice);
			DFS_articulacion(grafo, w, padre, visitados, orden, mas_alto, contador);
			mas_alto[atoi(vertice)] = minimo(mas_alto[atoi(vertice)], mas_alto[atoi(w)]);

		} else {

			if (padre[atoi(vertice)] != atoi(w)) mas_alto[atoi(vertice)] = minimo(mas_alto[atoi(vertice)], orden[atoi(w)]);
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(adyacentes, NULL);
}

/* *****************************
 *     BRANDES (Centralidad)
 * ****************************/

// Algoritmo de Centralidad.
int* calcular_centralidad(grafo_t* grafo) {

	int* centralidad_b = inicializar_centralidad(grafo);
	lista_t* vertices = grafo_vertices(grafo);
	lista_iter_t* iter = lista_iter_crear(vertices);

	while (!lista_iter_al_final(iter)) {

		char* vertice = lista_iter_ver_actual(iter);
		lista_t* *rho = inicializar_rho(grafo);
		pila_t* pila = pila_crear();
		int* sigma = inicializar_sigma(grafo);
		sigma[atoi(vertice)] = 1;
		int* distancia = inicializar_distancia(grafo);
		distancia[atoi(vertice)] = 0;
		lista_t* cola = lista_crear();
		lista_insertar_ultimo(cola, vertice);

		while (!lista_esta_vacia(cola)) {

			char* vertice_aux = lista_borrar_primero(cola);
			pila_apilar(pila, vertice_aux);
			lista_t* adyacentes = grafo_adyacentes(grafo, vertice_aux);
			lista_iter_t* iter1 = lista_iter_crear(adyacentes);

			while (!lista_iter_al_final(iter1)) {

				char* w = lista_iter_ver_actual(iter1);

				if (distancia[atoi(w)] < 0) {

					lista_insertar_ultimo(cola, w);
					distancia[atoi(w)] = distancia[atoi(vertice_aux)] + 1;
				}

				if (distancia[atoi(w)] == distancia[atoi(vertice_aux)] + 1) {

					sigma[atoi(w)] = sigma[atoi(w)] + sigma[atoi(vertice_aux)];
					lista_insertar_ultimo(rho[atoi(w)], vertice_aux);
				}

				lista_iter_avanzar(iter1);
			}

			lista_iter_destruir(iter1);
			lista_destruir(adyacentes, NULL);
		}

		lista_destruir(cola, NULL);

		int* delta = inicializar_delta(grafo);

		while (!pila_esta_vacia(pila)) {

			char* v = pila_desapilar(pila);
			lista_t* lista = rho[atoi(v)];
			lista_iter_t* iter2 = lista_iter_crear(lista);

			while (!lista_iter_al_final(iter2)) {

				char* x = lista_iter_ver_actual(iter2);
				delta[atoi(x)] = delta[atoi(x)] + sigma[atoi(x)]/sigma[atoi(v)]*(1 + delta[atoi(v)]);
				lista_iter_avanzar(iter2);
			}

			lista_iter_destruir(iter2);

			if (strcmp(v,vertice) != 0) centralidad_b[atoi(v)] = centralidad_b[atoi(v)] + delta[atoi(v)];
		}

		pila_destruir(pila);
		free(sigma);
		free(distancia);
		free(delta);
		destruir_rho(rho, grafo_cantidad_vertices(grafo));

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(vertices, NULL);

	return centralidad_b;
}

/* *****************************
 *          ANALIZAR
 * ****************************/

// Analiza la informacion difundida y devuelve una lista con los vertices en los cuales se difundio la misma.
lista_t* analizar_informacion(bool* informacion, int tam) {

	lista_t* lista = lista_crear();

	for (int i = 0; i < tam; i++) {

		if (informacion[i]) {
			
			int* j = malloc(sizeof(int));
			*j = i;
			lista_insertar_primero(lista, j);
		}
	}

	free(informacion);
	
	return lista;
}

// Analiza a partir de un vertice la componente conexa asociada.
int analizar_vertice(grafo_t* grafo, lista_t* cola_BFS, bool* visitados) {

	int contador = 1;
	
	while (!lista_esta_vacia(cola_BFS)) {

		char* vertice = lista_borrar_primero(cola_BFS);

		if (!visitado(visitados, vertice)) {

			lista_t* adyacentes_aux = grafo_adyacentes(grafo, vertice);
			lista_iter_t* iter_aux = lista_iter_crear(adyacentes_aux);

			while (!lista_iter_al_final(iter_aux)) {
				
				lista_insertar_primero(cola_BFS, lista_iter_ver_actual(iter_aux));
				lista_iter_avanzar(iter_aux);
			}

			lista_iter_destruir(iter_aux);
			lista_destruir(adyacentes_aux, NULL);
			
			visitados[atoi(vertice)] = true;
			contador++;
		}
	}

	return contador;
}

// Analiza si un vertice debe enterarse de la informacion o no.
void analizar_vertice_difundir(grafo_t* grafo, char* vertice, lista_t* adyacentes, bool* informacion) {

	int enterados = 0;

	lista_iter_t* iter = lista_iter_crear(adyacentes);
	
	while (!lista_iter_al_final(iter)) {

		if (informacion[atoi((char*)lista_iter_ver_actual(iter))]) {
			enterados++;
		}
		
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);

	if (enterados > grafo_cant_adyacentes(grafo, vertice)/2) informacion[atoi(vertice)] = true;
}

// Verifica si un vertice ya se enteró de la informacion a difundir.
bool difundido(bool* informacion, char* vertice) {

	return informacion[atoi(vertice)];
}

// Analiza la componente conexa donde se va a difundir la informacion.
void analizar_componente_a_difundir(grafo_t* grafo, lista_t* cola_BFS, char* vertice, bool* visitados, bool* informacion) {

	lista_t* adyacentes = grafo_adyacentes(grafo, vertice);

	if (!difundido(informacion, vertice)) analizar_vertice_difundir(grafo, vertice, adyacentes, informacion);

	lista_iter_t* iter = lista_iter_crear(adyacentes);

	while (!lista_iter_al_final(iter)) {

		lista_insertar_primero(cola_BFS, lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(adyacentes, NULL);


	while (!lista_esta_vacia(cola_BFS)) {

		char* vertice_aux = lista_borrar_primero(cola_BFS);

		lista_t* adyacentes_aux = grafo_adyacentes(grafo, vertice_aux);

		if (!visitado(visitados, vertice_aux)) {

			lista_iter_t* iter_aux = lista_iter_crear(adyacentes_aux);

			while (!lista_iter_al_final(iter_aux)) {

				lista_insertar_ultimo(cola_BFS, lista_iter_ver_actual(iter_aux));
				lista_iter_avanzar(iter_aux);
			}

			lista_iter_destruir(iter_aux);
			visitados[atoi(vertice_aux)] = true;
		}

		if (!difundido(informacion, vertice_aux)) analizar_vertice_difundir(grafo, vertice_aux, adyacentes_aux, informacion);

		lista_destruir(adyacentes_aux, NULL);
	}
}

// Cuenta cuantos hijos de un vertices del arbol DFS cumplen con la relacion que los convierte en puntos de articulacion.
int contar_vertices(int* padre, int* orden, int* mas_alto, char* vertice, int tam) {

	int  i = 0;
	
	for (int j = 0; j < tam; j++) {
		
		if (padre[j] == atoi(vertice)) {
			
			if (mas_alto[j] >= orden[atoi(vertice)]) i++;
		}
	}

	return i;
}

// Analiza que puntos son de articulacion a partir de la informacion proporcionada por el algoritmo de Tarjam.
void analizar_articulacion(bool* puntos_articulacion, char* vertice, int* padre, int* orden, int* mas_alto, lista_t* vertices_grafo, int tam) {

	char* vertice_aux;

	// Analiza la raiz del arbol DFS.
	int contador = 0;

	for (int i = 0; i < tam; i++) {
		if (padre[i] == atoi(vertice)) contador++;
	}
	
	if (contador != 1) puntos_articulacion[atoi(vertice)] = true;

	// Analiza los demás vertices del grafo que no son raiz del arbol DFS.
	lista_iter_t* iter = lista_iter_crear(vertices_grafo);

	while (!lista_iter_al_final(iter)) {

		vertice_aux = lista_iter_ver_actual(iter);
		
		if (strcmp(vertice, vertice_aux) != 0) {

			contador = contar_vertices(padre, orden, mas_alto, vertice_aux, tam);
			
			if (contador != 0) puntos_articulacion[atoi(vertice_aux)] = true;
		}
		
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
}

// Devuelve un vertice no visitado aun a partir del arreglo de visitados.
char* obtener_vertice(bool* visitados, size_t tam) {

	size_t i = 0;
	
	while (i < tam) {

		if (!visitados[i]) return itoa(i, 10);
		i++;
	}
	
	return NULL;
}

// Analiza si hay puntos de articulacion en el grafo usando el algoritmo de Tarjam para numerar el grafo en recorrido DFS.
// Tiene en cuenta las diferentes componenetes conexas, si es que las hay.
bool* analizar_puntos(grafo_t* grafo) {

	int tam = (int)grafo_cantidad_vertices(grafo);
	bool* puntos_articulacion = inicializar_articulacion(grafo);
	bool* visitados = inicializar_visitados(grafo);
	lista_t* vertices_grafo = grafo_vertices(grafo);

	while (!todos_visitados(visitados, tam)) {

		int* orden = inicializar_orden(grafo);
		int* padre = inicializar_padre(grafo);
		int* mas_alto = inicializar_mas_alto(grafo);
		int* contador = malloc(sizeof(int));
		*contador = 0;

		char* vertice = obtener_vertice(visitados, tam);

		DFS_articulacion(grafo, vertice, padre, visitados, orden, mas_alto, contador);

		analizar_articulacion(puntos_articulacion, vertice, padre, orden, mas_alto, vertices_grafo, tam);

		destruir(padre, orden, mas_alto, contador);
	}

	free(visitados);
	lista_destruir(vertices_grafo, NULL);
	
	return puntos_articulacion;
}

/* *****************************
 *      AUXILIARES VARIAS
 * ****************************/

// Crea una estructura central que contiene un vertice y la cantidad de veces que apaerece en los caminos minimos.
central_t* crear_central(int vertice, int apariciones) {

	central_t* central = malloc(sizeof(central_t));
	
	if (!central) return NULL;

	central->vertice = malloc(sizeof(int));
	
	if (!central->vertice) {
		
		free(central);
		return NULL;
	}
	
	*central->vertice = vertice;

	central->apariciones = malloc(sizeof(int));
	
	if (!central->apariciones) {
		
		free(central->vertice);
		free(central);
		
		return NULL;
	}

	*central->apariciones = apariciones;

	return central;
}

// Verifica que los vertices a difundir pertenezcan al grafo.
bool verficar_vertices_a_difundir(lista_t* lista, grafo_t* grafo) {

	lista_iter_t* iter = lista_iter_crear(lista);

	while (!lista_iter_al_final(iter)) {

		if (!grafo_pertence(grafo, lista_iter_ver_actual(iter))) {
			
			lista_iter_destruir(iter);
			
			return false;
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	
	return true;
}

// Devuelve la posicion donde se encuentra el camino mas largo.
int camino_maximo(vector_t* caminos) {

	int pos = 0; int i = 0; size_t largo;

	largo = lista_largo((lista_t*)vector_obtener(caminos, i));
	i++;

	while (i < vector_obtener_cantidad(caminos)) {

		if (lista_largo((lista_t*)vector_obtener(caminos, i)) > largo) {

			largo = lista_largo((lista_t*)vector_obtener(caminos, i));
			pos = i;
		}
		
		i++;
	}

	return pos;
}

// A partir de todos los caminos minimos, busca el que posee mayor largo.
lista_t* camino_diametro(vector_t* caminos) {

	int pos_maximo = camino_maximo(caminos);
	
	lista_t* maximo = vector_obtener(caminos, pos_maximo);
	
	lista_t* lista = copiar_lista(maximo);
	
	return lista;
}

// Devuelve una lista con los puntos de articulacion teniendo en cuenta un arreglo de bool.
lista_t* extraer_puntos(bool* puntos, int tam) {

	lista_t* lista = lista_crear();

	for (int i = 0; i < tam; i++) {

		if (puntos[i]) {

			int* j = malloc(sizeof(int));
			*j = i;
			lista_insertar_primero(lista, j);
		}
	}

	free(puntos);
	
	return lista;
}

// Extrae a partir del arreglo de centrales la cantidad pedida.
lista_t* extraer_mas_centrales(int* centrales, char* cantidad, int tam) {

	lista_t* lista = lista_crear();
	heap_t* heap_min = heap_crear(cmp1, atoi(cantidad));

	/* Encola en el heap hasta que la cantidad sea la especificada */
	int i = 0; int j = 0;
	
	while (i < atoi(cantidad)) {

		if (centrales[j] != 0) {
			
			central_t* central = crear_central(j, centrales[j]);
			heap_encolar(heap_min, central);
			i++;
		}
		
		j++;
	}

	/* Sigue iterando el arreglo y compara cada elemento con el tope del heap.
		Si el elemento es mayor que el tope, se desencola el tope y se guarda el elemento.
		Sino se descarta.
	*/
	while (j < tam) {

		central_t* central = heap_ver_max(heap_min);
		
		if (centrales[j] > *(central->apariciones)) {
			
			destruir_central(heap_desencolar(heap_min));
			central_t* central_aux = crear_central(j, centrales[j]);
			heap_encolar(heap_min, central_aux);
		}
		
		j++;
	}

	/* Los desencola del heap y los inserta en una lista porque el heap es de minimos y tienen que salir en oreden decreciente*/
	while (!heap_esta_vacio(heap_min)) {

		lista_insertar_primero(lista, heap_desencolar(heap_min));
	}

	heap_destruir(heap_min, NULL);

	free(centrales);
	
	return lista;
}

/* ******************************************************
 *                       COMANDOS
 * *****************************************************/

/* *****************************
 *          RECOMENDAR
 * ****************************/

// Devuelve una lista con las posibles recomendaciones para el vertice.
lista_t* posibles_recomendaciones(grafo_t* grafo, char* vertice) {

	lista_t* lista = lista_crear();

	lista_t* adyacentes = grafo_adyacentes(grafo, vertice);

	lista_iter_t* iter = lista_iter_crear(adyacentes);
	lista_t* adyacentes_aux;

	while (!lista_iter_al_final(iter)) {

		adyacentes_aux = grafo_adyacentes(grafo, lista_iter_ver_actual(iter));
		
		if (adyacentes_aux) {
			
			lista = union_lista(lista, adyacentes_aux);
			lista_destruir(adyacentes_aux, NULL);
		}
		
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(adyacentes, NULL);

	return lista;
}

// Devuelve una lista con los vertices recomendados.
lista_t* recomendados(grafo_t* grafo, char* vertice, char* cantidad, lista_t* posibles_recomend) {

	lista_t* lista = lista_crear();

	int* arreglo_aux = malloc(sizeof(int)* grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo_aux[i] = 0;
	}

	lista_iter_t* iter = lista_iter_crear(posibles_recomend);
	
	while (!lista_iter_al_final(iter)) {
		
		if (strcmp(lista_iter_ver_actual(iter), vertice) != 0 && !grafo_adyacente_pertence(grafo, vertice, lista_iter_ver_actual(iter))) {
			(arreglo_aux[atoi((char*)lista_iter_ver_actual(iter))])++;
		}
		
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(posibles_recomend, NULL);

	heap_t* maximos = heap_crear(cmp1, atoi(cantidad));

	int i = 0; int j = 0;
	
	while (i < atoi(cantidad)) {
		
		if (arreglo_aux[j] != 0) {
			
			int* k = malloc(sizeof(int));
			*k = j;
			heap_encolar(maximos, k);
			i++;
		}

		j++;
	}

	while (j < grafo_cantidad_vertices(grafo)) {
		
		int* i = heap_ver_max(maximos);
		
		if (arreglo_aux[j] > *i) {
			
			free(heap_desencolar(maximos));
			int* k = malloc(sizeof(int));
			*k = j;
			heap_encolar(maximos, k);
		}
		
		j++;
	}

	while (!heap_esta_vacio(maximos)) {
		lista_insertar_ultimo(lista, heap_desencolar(maximos));
	}

	free(arreglo_aux);
	heap_destruir(maximos, NULL);
	
	return lista;
}

// Devuelve una lista con los vertices a recomendar.
lista_t* recomendaciones(grafo_t* grafo, char* vertice, char* cantidad) {

	lista_t* posibles_recomendados = posibles_recomendaciones(grafo, vertice);

	if (lista_esta_vacia(posibles_recomendados)) {
		return posibles_recomendados;
	}

	return recomendados(grafo, vertice, cantidad, posibles_recomendados);
}

/* *****************************
 *          DIFUNDIR
 * ****************************/

// Defunde la informacion en el grafo.
bool* difundir_informacion(grafo_t* grafo, lista_t* lista, int tam) {

	bool* informacion = inicializacion_difundir(lista, tam);

	bool* visitados = inicializar_visitados(grafo);

	lista_t* cola_BFS = lista_crear();

	while (!todos_visitados(visitados, grafo_cantidad_vertices(grafo))) {

		char* vertice = obtener_vertice(visitados, grafo_cantidad_vertices(grafo));

		visitados[atoi(vertice)] = true;

		analizar_componente_a_difundir(grafo, cola_BFS, vertice, visitados, informacion);
	}

	lista_destruir(cola_BFS, NULL);

	free(visitados);

	return informacion;
}

// Devuelve una lista con los vertices en los cuales se difundio la informacion.
lista_t* difundir_grafo(grafo_t* grafo, lista_t* lista) {

	int tam = (int)grafo_cantidad_vertices(grafo);

	bool* informacion = difundir_informacion(grafo, lista, tam);

	lista_t* difundidos = analizar_informacion(informacion, tam);

	return difundidos;
}

/* *****************************
 *          CENTRALIDAD
 * ****************************/

// Devuelve una lista con los vertices mas centrales.
lista_t* centralidad_grafo(grafo_t* grafo, char* cantidad) {

	int* centrales = calcular_centralidad(grafo);

	return extraer_mas_centrales(centrales, cantidad, grafo_cantidad_vertices(grafo));
}

/* *****************************
 *          SUBGRUPOS
 * ****************************/

// Calcula las componentes conexas del grafo.
heap_t* calcular_subgrupos(grafo_t* grafo) {

	lista_t* cola_BFS = lista_crear();
	bool* visitados = inicializar_visitados(grafo);
	heap_t* componentes = heap_crear(cmp2, grafo_cantidad_vertices(grafo));

	while (!todos_visitados(visitados, grafo_cantidad_vertices(grafo))) {

		char* vertice = obtener_vertice(visitados, grafo_cantidad_vertices(grafo));
		lista_t* adyacentes = grafo_adyacentes(grafo, vertice);
		lista_iter_t* iter = lista_iter_crear(adyacentes);

		visitados[atoi(vertice)] = true;
		
		while (!lista_iter_al_final(iter)) {
			
			lista_insertar_primero(cola_BFS, lista_iter_ver_actual(iter));
			lista_iter_avanzar(iter);
		}
		
		lista_iter_destruir(iter);
		lista_destruir(adyacentes, NULL);

		int vertices_analizados = analizar_vertice(grafo, cola_BFS, visitados);
		int* componente_conexa = malloc(sizeof(int));
		*componente_conexa = vertices_analizados;
		heap_encolar(componentes, componente_conexa);
	}

	lista_destruir(cola_BFS, NULL);
	free(visitados);
	
	return componentes;
}

/* *****************************
 *          DIAMETRO
 * ****************************/

// Devuelve una lista con los vertices que componen el camino maximo de todos los caminos minimos.
lista_t* calcular_diametro(grafo_t* grafo) {

	vector_t* caminos = calcular_caminos(grafo);
	lista_t* camino_max = camino_diametro(caminos);
	
	destruir_vector(caminos);
	
	return camino_max;
}

/* *****************************
 *    PUNTOS DE ARTICULACION
 * ****************************/

// Calcula los puntos de articulacion del grafo.
lista_t* calcular_puntos_articulacion(grafo_t* grafo) {

	bool* puntos = analizar_puntos(grafo);

	return extraer_puntos(puntos, (int)grafo_cantidad_vertices(grafo));
}

/* *****************************
 *        AGRUPAMIENTO
 * ****************************/

// Inicializa un arreglo con ceros para el clustering
float* inicializar_clustering(grafo_t* grafo) {

	float* arreglo = malloc(sizeof(float)*grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = 0.0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con ceros para el hash
int* inicializar_hash(grafo_t* grafo) {

	int* arreglo = malloc(sizeof(int)*grafo_cantidad_vertices(grafo));

	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo[i] = 0;
	}

	return arreglo;
}

// Hashea vertices para luego contar las uniones entre ellos
void hashear_vertices(char* vertice, lista_t* adyacentes, int* h) {

	lista_iter_t* iter = lista_iter_crear(adyacentes);

	while (!lista_iter_al_final(iter)) {

		char* v = lista_iter_ver_actual(iter);

		if (strcmp(v, vertice) != 0) {

			h[atoi(v)] = h[atoi(v)] + 1;
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
}

float contar_cantidad_de_uniones(grafo_t* grafo, char* vertice, lista_t* adyacentes, lista_t* adyacentesA) {

	float cont = 0.0;

	int* h = inicializar_hash(grafo);

	hashear_vertices(vertice, adyacentesA, h);

	hashear_vertices(vertice, adyacentes, h);

	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {

		if (h[i] > 1) cont++;
	}

	free(h);

	return cont;
}

// Calcula cuantas uniones hay entre los vertices adyacentes a otro.
float uniones_adyacentes(grafo_t* grafo, char* vertice, lista_t* adyacentes) {

	float contador = 0.0;

	lista_iter_t* iter = lista_iter_crear(adyacentes);
	
	while (!lista_iter_al_final(iter)) {

		char* verticeA = lista_iter_ver_actual(iter);

		lista_t* adyacentesA = grafo_adyacentes(grafo, verticeA);

		contador = contador + contar_cantidad_de_uniones(grafo, vertice, adyacentes, adyacentesA);

		lista_destruir(adyacentesA, NULL);

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);

	return contador;
}

// Calacula el coeficiente de agrupamiento para cada verice del grafo.
float* agrupamiento_vertices(grafo_t* grafo) {

	float* clustering_v = inicializar_clustering(grafo);

	lista_t* vertices = grafo_vertices(grafo);
	lista_iter_t* iter = lista_iter_crear(vertices);

	while (!lista_iter_al_final(iter)) {

		char* vertice = lista_iter_ver_actual(iter);
		
		lista_t* adyacentes = grafo_adyacentes(grafo, vertice);

		if (grafo_cant_adyacentes(grafo, vertice) == 1) {
			
			clustering_v[atoi(vertice)] = 0.0;

		} else {
			
			float e = uniones_adyacentes(grafo, vertice, adyacentes);
			float k = grafo_cant_adyacentes(grafo, vertice)*(grafo_cant_adyacentes(grafo, vertice) - 1);

			clustering_v[atoi(vertice)] = e/k;
		}
		
		lista_destruir(adyacentes, NULL);
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(vertices, NULL);

	return clustering_v;
}

// Calcula el coeficiente de agrupamiento para todo el grafo a partir del de cada uno de los vertices.
float agrupamiento_total(grafo_t* grafo, float* clustering_v) {

	float contador = 0;

	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		contador = contador + clustering_v[i];
	}

	free(clustering_v);
	
	return contador/grafo_cantidad_vertices(grafo);
}

// Calcula el coeficiente de agrupamiento/clustering del grafo.
float calcular_agrupamiento(grafo_t* grafo) {

	float* clustering_v = agrupamiento_vertices(grafo);

	return agrupamiento_total(grafo, clustering_v);
}

/* ******************************************************************
 *              RECOMENDAR    DIFUNDIR    CENTRALIDAD
 * *****************************************************************/

void recomendar(grafo_t* grafo, lista_t* lista) {

	char* vertice = lista_borrar_primero(lista);
	
	if (!vertice) {
		
		printf("ERROR VERTICE INVALIDO\n");
		return;
	}

	char* cantidad = lista_borrar_primero(lista);
	
	if (!cantidad) {
		
		free(vertice);
		printf("ERROR CANTIDAD INVALIDA\n");
		
		return;
	}

	if (atoi(cantidad) == 0) {
		
		free(cantidad);
		free(vertice);
		printf("ERROR CANTIDAD INVALIDA\n");
		
		return;
	}

	lista_t* recomendados = recomendaciones(grafo, vertice, cantidad);

	if (lista_esta_vacia(recomendados)) {
		
		lista_destruir(recomendados, NULL);
		printf("NO HAY MUSICOS PARA RECOMENDAR\n");
		free(vertice);
		free(cantidad);
		
		return;
	}

	lista_iter_t* iter = lista_iter_crear(recomendados);
	printf("Las recomendaciones para el musico %s son:\n", vertice);

	while (!lista_iter_al_final(iter)) {
		
		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(recomendados, free);
	free(vertice);
	free(cantidad);
}

void difundir(grafo_t* grafo, lista_t* lista) {

	if (!lista_ver_primero(lista)) {
		
		printf("ERROR MUSICOS A DIFUNDIR INVALIDOS\n");
		return;
	}

	if (!verficar_vertices_a_difundir(lista, grafo)) {
		
		printf("ERROR ALGUN MUSICO A DIFUNDIR NO EXISTE\n");
		return;
	}

	lista_t* difundidos = difundir_grafo(grafo, lista);

	lista_iter_t* iter = lista_iter_crear(difundidos);
	printf("La cantidad de musicos a los cuales se difundio la informacion es %d\n", (int)lista_largo(difundidos));
	printf("Estos musicos son:\n");

	while (!lista_iter_al_final(iter)) {
		
		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(difundidos, free);
}

void centralidad(grafo_t* grafo, lista_t* lista) {

	char* cantidad = lista_borrar_primero(lista);
	
	if (!cantidad || atoi(cantidad) <= 0) {
		
		printf("ERROR CANTIDAD INVALIDA\n");
		return;
	}

	lista_t* centrales = centralidad_grafo(grafo, cantidad);
	
	lista_iter_t* iter = lista_iter_crear(centrales);
	printf("Los músicos mas centrales son:\n");

	while (!lista_iter_al_final(iter)) {

		central_t* central = lista_iter_ver_actual(iter);
		printf("Vertice: %d, Centralidad: %d\n", *central->vertice, *central->apariciones);
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(centrales, destruir_central);
	free(cantidad);
}

/* ******************************************************************
 *     DIAMETRO   CAMINO   SUBGRUPOS  ARTICULACION  AGRUPAMIENTO
 * *****************************************************************/

void diametro(grafo_t* grafo) {

	lista_t* camino_diametro = calcular_diametro(grafo);
	lista_iter_t* iter = lista_iter_crear(camino_diametro);

	printf("El diametro es %d\n", (int)lista_largo(camino_diametro)-1);
	printf("Los músicos que componen el camino son:\n");

	while (!lista_iter_al_final(iter)) {
		
		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	lista_destruir(camino_diametro, free);
}

void camino(grafo_t* grafo, lista_t* lista) {

	char* vertice1 = lista_borrar_primero(lista);
	
	if (!vertice1) {
		
		printf("ERROR COMANDO INVALIDO\n");
		return;
	}
	
	if (!grafo_pertence(grafo, vertice1)) {
		
		printf("MUSICO INEXISTENTE\n");
		free(vertice1);
		
		return;
	}
	
	char* vertice2 = lista_borrar_primero(lista);
	
	if (!vertice2) {
		
		free(vertice1);
		printf("ERROR COMANDO INVALIDO\n");
		
		return;
	}
	
	if (!grafo_pertence(grafo, vertice2)) {
		
		printf("MUSICO INEXISTENTE\n");
		free(vertice2);
		free(vertice1);
		
		return;
	}

	lista_t* recorrido = calcular_camino(grafo, vertice1, vertice2);
	
	if (!recorrido) {
		
		printf("EL CAMINO ENTRE LOS MUSICOS NO EXISTE\n");
		return;
	}

	lista_iter_t* iter = lista_iter_crear(recorrido);
	printf("El camino entre %s y %s es:\n", vertice1, vertice2);

	while (!lista_iter_al_final(iter)) {
		
		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	lista_destruir(recorrido, free);
	free(vertice1);
	free(vertice2);
}

void subgrupos(grafo_t* grafo) {

	heap_t* grupos = calcular_subgrupos(grafo);
	
	printf("La cantidad de subgrupos/componentes conexas es: %d\n", (int)heap_cantidad(grupos));
	printf("Cantidad de musicos por subgrupo\n");

	int i = 1;
	
	while (!heap_esta_vacio(grupos)) {
		printf("Subgrupo %d\n", i);
		int* subgrupo = heap_desencolar(grupos);
		printf("%d\n", *subgrupo);
		free(subgrupo);
		i++;
	}

	heap_destruir(grupos, NULL);
}

void articulacion(grafo_t* grafo) {

	lista_t* puntos = calcular_puntos_articulacion(grafo);

	if (lista_esta_vacia(puntos)) {

		printf("El grafo no contiene puntos de articulacion\n");
		lista_destruir(puntos, NULL);
		
		return;
	}

	lista_iter_t* iter = lista_iter_crear(puntos);
	printf("Los puntos de articulacion son:\n");

	while (!lista_iter_al_final(iter)) {

		printf("%d\n", *(int*)lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(puntos, free);
}

void agrupamiento(grafo_t* grafo) {

	printf("El coeficiente de agrupamiento/clustering es %f\n", calcular_agrupamiento(grafo));
}

/* ******************************************************************
 *                         PROCESAR LINEA
 * *****************************************************************/

void procesar_linea(lista_t* lista, grafo_t* grafo) {

	if (!lista_esta_vacia(lista)) {

		char* comando = lista_borrar_primero(lista);
		
		if (!comando) {
			
			printf("ERROR COMANDO INVALIDO\n");
			return;
		}

		if (strcmp(comando, RECOMENDAR) == 0) {

			recomendar(grafo, lista);

		} else if (strcmp(comando, DIFUNDIR) == 0) {

			difundir(grafo, lista);

		} else if (strcmp(comando, CENTRALIDAD) == 0) {

			centralidad(grafo, lista);

		} else if (strcmp(comando, CAMINO) == 0) {

			camino(grafo, lista);

		} else if (strcmp(comando, DIAMETRO) == 0) {

			diametro(grafo);

		} else if (strcmp(comando, SUBGRUPOS) == 0) {

			subgrupos(grafo);

		} else if (strcmp(comando, ARTICULACION) == 0) {

			articulacion(grafo);

		} else if (strcmp(comando, AGRUPAMIENTO) == 0) {

			agrupamiento(grafo);

		} else {
			
			printf("ERROR COMANDO INVALIDO\n");
		}

		free(comando);
	}
}

/* ******************************************************************
 *                              TP3
 * *****************************************************************/

void tp3() {

	grafo_t* grafo = cargar_grafo();
	
	if (!grafo) {
		
		printf("ERROR INESPERADO\n");
		return;
	}

	if (grafo_esta_vacio(grafo)) {
		
		grafo_destruir(grafo);
		return;
	}

	printf("GRAFO CARGADO\n\n");

	lista_t* linea = leer_linea();
	
	while (linea != NULL) {

		procesar_linea(linea, grafo);
		destruir_linea(linea);
		linea = leer_linea();
	}

	grafo_destruir(grafo);
}

int main(void) {

	tp3();
	return 0;
}