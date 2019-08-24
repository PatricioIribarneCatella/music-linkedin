#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "grafo.h"
#include "lista.h"

#include "utils.h"
#include "tarjam.h"
#include "visitar.h"
#include "inicializar.h"
#include "destruir.h"

// Devuelve un vértice no visitado aún
// a partir del arreglo de visitados.
char* obtener_vertice(bool* visitados, size_t tam) {

	size_t i = 0;
	
	while (i < tam) {

		if (!visitados[i])
			return itoa(i, 10);
		i++;
	}
	
	return NULL;
}

/* *****************************
 *          SUBGRAFOS
 * ****************************/

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

/* *****************************
 *          DIFUNDIR
 * ****************************/

// Analiza si un vertice debe enterarse de la informacion o no.
static void analizar_vertice_difundir(grafo_t* grafo,
				char* vertice,
				lista_t* adyacentes,
				bool* informacion) {

	int enterados = 0;

	lista_iter_t* iter = lista_iter_crear(adyacentes);
	
	while (!lista_iter_al_final(iter)) {

		if (informacion[atoi((char*)lista_iter_ver_actual(iter))]) {
			enterados++;
		}
		
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);

	if (enterados > grafo_cant_adyacentes(grafo, vertice)/2)
		informacion[atoi(vertice)] = true;
}

// Verifica si un vertice ya se enteró de la informacion a difundir.
static bool difundido(bool* informacion, char* vertice) {

	return informacion[atoi(vertice)];
}

// Analiza la componente conexa donde se va a difundir la informacion.
void analizar_componente_a_difundir(grafo_t* grafo, lista_t* cola_BFS, char* vertice, bool* visitados, bool* informacion) {

	lista_t* adyacentes = grafo_adyacentes(grafo, vertice);

	if (!difundido(informacion, vertice))
		analizar_vertice_difundir(grafo, vertice, adyacentes, informacion);

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

		if (!difundido(informacion, vertice_aux))
			analizar_vertice_difundir(grafo, vertice_aux, adyacentes_aux, informacion);

		lista_destruir(adyacentes_aux, NULL);
	}
}

// Analiza la informacion difundida y
// devuelve una lista con los vértices
// en los cuales se difundio la misma.
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

/* *****************************
 *        ARTICULACION
 * ****************************/

// Cuenta cuántos hijos de un vértice del
// arbol DFS cumplen con la relación que
// los convierte en puntos de articulacion.
static int contar_vertices(int* padre, int* orden,
			int* mas_alto,
			char* vertice,
			int tam) {

	int  i = 0;
	
	for (int j = 0; j < tam; j++) {
		
		if (padre[j] == atoi(vertice)) {
			
			if (mas_alto[j] >= orden[atoi(vertice)])
				i++;
		}
	}

	return i;
}

// Analiza qué puntos son de articulación
// a partir de la información proporcionada
// por el algoritmo de Tarjam.
static void analizar_articulacion(bool* puntos_articulacion,
			   char* vertice,
			   int* padre,
			   int* orden,
			   int* mas_alto,
			   lista_t* vertices_grafo,
			   int tam) {

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


// Analiza si hay puntos de articulación en el grafo
// usando el algoritmo de Tarjam para numerar
// el grafo en recorrido DFS.
// 
// Tiene en cuenta las diferentes
// componenetes conexas, si es que las hay.
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

