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

