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
/* *****************************
 *    PUNTOS DE ARTICULACION
 * ****************************/

// Calcula los puntos de articulacion del grafo.
lista_t* calcular_puntos_articulacion(grafo_t* grafo) {

	bool* puntos = analizar_puntos(grafo);

	return extraer_puntos(puntos, (int)grafo_cantidad_vertices(grafo));
}

