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


