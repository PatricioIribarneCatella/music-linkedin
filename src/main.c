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

#include "destruir.h"
#include "parsing.h"
#include "comandos.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct central {
	int* vertice;
	int* apariciones;
} central_t;

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

// Destruye la estructura de central.
void destruir_central(void* dato) {

	central_t* central = (central_t*)dato;

	free(central->vertice);
	free(central->apariciones);
	free(central);
}

/* *****************************
 *            ITOA
 * ****************************/

// Convierte un entero a string.
// Fuente: http://newbiegeek.com/convertir-un-entero-a-cadena-de-caracteres-en-c/
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

		if (!visitados[i])
			return false;
		i++;
	}

	return true;
}

/* *****************************
 *         COMPARACION
 * ****************************/

// Función de comparación 1 (heap de minimos).
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

// Funcion de comparacion 2 (heap de maximos).
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

// Funcion de comparacion 3 (heap de maximos guardando estructura central).
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

/* *****************************
 *       INICIALIZACION
 * ****************************/

// Inicializa un arreglo con false para los visitados.
bool* inicializar_visitados(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	bool* visita = malloc(sizeof(bool)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		visita[i] = false;
	}
	
	return visita;
}

// Inicializa un arreglo con la
// cantidad de vertices en cada posicion para los padres.
int* inicializar_padre(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	int* padre = malloc(sizeof(int)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		padre[i] = cant_vertices;
	}
	
	return padre;
}

// Inicializa un arreglo con ceros para el orden.
int* inicializar_orden(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	int* orden = malloc(sizeof(int)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		orden[i] = 0;
	}
	
	return orden;
}

// Inicializa un arreglo con ceros para mas_alto.
int* inicializar_mas_alto(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	int* mas_alto = malloc(sizeof(int)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		mas_alto[i] = 0;
	}
	
	return mas_alto;
}

// Inicializa un arreglo con false para los puntos de articulacion.
bool* inicializar_articulacion(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	bool* ptos_articulacion = malloc(sizeof(bool)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		ptos_articulacion[i] = false;
	}
	
	return ptos_articulacion;
}

// Inicializa el contador en cero y
// registra los vértices donde se va
// plantar la informacion a difundir.
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

	int cant_vertices = grafo_cantidad_vertices(grafo);

	int* arreglo = malloc(sizeof(int)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		arreglo[i] = 0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con 0 para el parametro Sigma.
int* inicializar_sigma(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	int* arreglo = malloc(sizeof(int)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		arreglo[i] = 0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con -1 para la distancia.
int* inicializar_distancia(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	int* arreglo = malloc(sizeof(int)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		arreglo[i] = -1;
	}
	
	return arreglo;
}

// Inicializa un arreglo con 0 para el parametro Delta.
int* inicializar_delta(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	int* arreglo = malloc(sizeof(int)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		arreglo[i] = 0;
	}
	
	return arreglo;
}

// Inicializa un arreglo con listas vacias para el parametro Rho.
lista_t* *inicializar_rho(grafo_t* grafo) {

	int cant_vertices = grafo_cantidad_vertices(grafo);

	lista_t* *arreglo = malloc(sizeof(lista_t*)*cant_vertices);
	
	for (int i = 0; i < cant_vertices; i++) {
		arreglo[i] = lista_crear();
	}
	
	return arreglo;
}





/* *****************************
 *      AUXILIARES VARIAS
 * ****************************/

// Crea una estructura central que contiene un vertice
// y la cantidad de veces que apaerece en los caminos minimos.
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
	
	lista_t* lista = lista_copiar(maximo);
	
	return lista;
}

// Devuelve una lista con los puntos de articulación
// teniendo en cuenta un arreglo de bool.
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

	/* Los desencola del heap y los inserta en una lista
	 * porque el heap es de minimos y tienen que
	 * salir en oreden decreciente
	 */
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
			
			lista = lista_union(lista, adyacentes_aux);
			lista_destruir(adyacentes_aux, NULL);
		}
		
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(adyacentes, NULL);

	return lista;
}

// Devuelve una lista con los vertices recomendados.
lista_t* recomendados(grafo_t* grafo,
		      char* vertice,
		      char* cantidad,
		      lista_t* posibles_recomend) {

	lista_t* lista = lista_crear();

	int* arreglo_aux = malloc(sizeof(int)* grafo_cantidad_vertices(grafo));
	
	for (int i = 0; i < grafo_cantidad_vertices(grafo); i++) {
		arreglo_aux[i] = 0;
	}

	lista_iter_t* iter = lista_iter_crear(posibles_recomend);
	
	while (!lista_iter_al_final(iter)) {
		
		if (strcmp(lista_iter_ver_actual(iter), vertice) != 0 &&
				!grafo_adyacente_pertence(grafo, vertice, lista_iter_ver_actual(iter))) {
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





/* ******************************************************************
 *                         Music-LinkeIn
 * *****************************************************************/

void linkedin() {

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

	linkedin();

	return 0;
}

