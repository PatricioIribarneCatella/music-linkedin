#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"
#include "lista.h"

#include "comandos.h"

#include "recomendar.h"
#include "difundir.h"
#include "centralidad.h"
#include "camino.h"
#include "diametro.h"
#include "subgrafos.h"
#include "articulacion.h"
#include "agrupamiento.h"

/* ******************************************************************
 *                         PROCESAR LINEA
 * *****************************************************************/

void procesar_linea(lista_t* lista, grafo_t* grafo) {

	if (!lista_esta_vacia(lista)) {

		char* comando = lista_borrar_primero(lista);
		
		if (!comando) {
			
			printf("ERROR COMANDO INVALIDO\n");
			return;
		}

		if (strcmp(comando, RECOMENDAR) == 0) {

			recomendar(grafo, lista);

		} else if (strcmp(comando, DIFUNDIR) == 0) {

			difundir(grafo, lista);

		} else if (strcmp(comando, CENTRALIDAD) == 0) {

			centralidad(grafo, lista);

		} else if (strcmp(comando, CAMINO) == 0) {

			camino(grafo, lista);

		} else if (strcmp(comando, DIAMETRO) == 0) {

			diametro(grafo);

		} else if (strcmp(comando, SUBGRUPOS) == 0) {

			subgrupos(grafo);

		} else if (strcmp(comando, ARTICULACION) == 0) {

			articulacion(grafo);

		} else if (strcmp(comando, AGRUPAMIENTO) == 0) {

			agrupamiento(grafo);

		} else {
			
			printf("ERROR COMANDO INVALIDO\n");
		}

		free(comando);
	}
}

