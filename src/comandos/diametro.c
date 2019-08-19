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

/* *****************************
 *          DIAMETRO
 * ****************************/

// Devuelve una lista con los vertices que componen
// el camino maximo de todos los caminos minimos.
lista_t* calcular_diametro(grafo_t* grafo) {

	vector_t* caminos = calcular_caminos(grafo);
	lista_t* camino_max = camino_diametro(caminos);
	
	destruir_vector(caminos);
	
	return camino_max;
}

