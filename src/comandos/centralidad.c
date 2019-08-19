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


