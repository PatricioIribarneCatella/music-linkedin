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



