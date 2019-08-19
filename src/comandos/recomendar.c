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


