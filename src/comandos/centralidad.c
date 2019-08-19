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
		printf("Vertice: %d, Centralidad: %d\n",
				*central->vertice, *central->apariciones);
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(centrales, destruir_central);
	free(cantidad);
}

/* *****************************
 *          CENTRALIDAD
 * ****************************/

// Devuelve una lista con los vertices mas centrales.
lista_t* centralidad_grafo(grafo_t* grafo, char* cantidad) {

	int* centrales = calcular_centralidad(grafo);

	return extraer_mas_centrales(centrales, cantidad, grafo_cantidad_vertices(grafo));
}

