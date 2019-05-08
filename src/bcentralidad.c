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

			if (strcmp(v,vertice) != 0)
				centralidad_b[atoi(v)] = centralidad_b[atoi(v)] + delta[atoi(v)];
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

