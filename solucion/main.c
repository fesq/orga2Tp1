#include <stdio.h>
#include "tdt.h"
#include "auxiliares.h"

void printMaxMin(tdt* tabla);
bloque* crearBloque(char* clave, char* valor);

int main(void) {
	/**
	 * Ej 2:
	 */

	// 1 :
	tdt* tabla = tdt_crear("pepe"); // rarest pepe


	// 2 :
	uint8_t claveB[3] = { 255, 255, 255 };
	uint8_t claveA[3] = { 0, 0, 0 };

	uint8_t valorB[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t valorA[15] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255 };

	// Agregamos 00 00 00 : FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
	tdt_agregar(tabla, claveA, valorA);

	// Agregamos FF FF FF : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	tdt_agregar(tabla, claveB, valorB);


	// 3 :
	bloque** bloques = malloc(5 * sizeof(bloque*));
	bloques[0] = crearBloque("050505", "123456789ABCDEF123456789ABCDEF");
	bloques[1] = crearBloque("FFFFFF", "112233445566778899AABBCCDDEEFF");
	bloques[2] = crearBloque("53FFAA", "111222333444555666777888999AAA");
	bloques[3] = crearBloque("10EE05", "111122223333444455556666777788");
	bloques[4] = 0;

	tdt_agregarBloques(tabla, bloques);

	for (int i = 0; i < 4; i++) {
		free(bloques[i]);
	}
	free(bloques);


	// 4 :
	bloques = malloc(3 * sizeof(bloque*));
	bloques[0] = crearBloque("FFFFFF", "112233445566778899AABBCCDDEEFF");
	bloques[1] = crearBloque("53FFAA", "111222333444555666777888999AAA");
	bloques[2] = 0;
	tdt_borrarBloques(tabla, bloques);

	for (int i = 0; i < 2; i++) {
		free(bloques[i]);
	}
	free(bloques);


	// 5 :
	printMaxMin(tabla);


	// 6 :
	FILE* file = fopen("traducciones.txt", "w+");
	tdt_imprimirTraducciones(tabla, file);
	fclose(file);


	// 7 :
	printf("\nCantidad: %d", tdt_cantidad(tabla));


	// 8 :
	tdt_destruir(&(tabla));

	return 0;
}


void printMaxMin(tdt* tabla) {
	maxmin* maxMin = tdt_obtenerMaxMin(tabla);
	printf("\n");

	printf("Max clave:");
	printInt8Array(maxMin->max_clave, 3);
	printf("\n");

	printf("Max valor:");
	printInt8Array(maxMin->max_valor, 15);
	printf("\n");

	printf("Min clave:");
	printInt8Array(maxMin->min_clave, 3);
	printf("\n");

	printf("Min valor:");
	printInt8Array(maxMin->min_valor, 15);
	printf("\n");
	free(maxMin);
}

bloque* crearBloque(char* clave, char* valor) {
	bloque* bloq = (bloque*) malloc(sizeof(bloque) * 1);
	uint8_t claveBloque[3];
	hexString_a_int8Array(clave, 6, claveBloque);
	memcpy(bloq->clave, claveBloque, 3);
	uint8_t valorBloque[15];
	hexString_a_int8Array(valor, 30, valorBloque);
	memcpy(bloq->valor, valorBloque, 15);
	return bloq;
}
