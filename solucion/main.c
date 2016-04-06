#include <stdio.h>
#include "tdt.h"
#include "auxiliares.h"

int main(void) {

	tdt* tabla = tdt_crear("sa");
	uint8_t claveB[3];
	uint8_t valorB[15];
	claveB[0] = 255;
	claveB[1] = 255;
	claveB[2] = 255;

	uint8_t claveA[3];
	uint8_t valorA[15];
	claveA[0] = 0;
	claveA[1] = 0;
	claveA[2] = 0;

	uint8_t valorC[15];
	for(int i = 0; i<15; i++){
		valorA[i] = 255;
		valorB[i] = 0;
		valorC[i] = 0;
	}

	tdt_agregar(tabla,claveA,valorA);
	tdt_agregar(tabla,claveB,valorB);

	char trad[30];

	tdt_traducir(tabla, claveA, valorC);
	int8Array_a_HexString(valorC,15, trad);
	for(int i = 0; i<15; i++){
		printf("%c",trad[i]);
	}

	tdt_borrar(tabla, claveB);

	tdt_destruir(&(tabla));

	return 0;
}
