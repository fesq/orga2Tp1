#include <stdio.h>
#include "tdt.h"

int main(void) {

	tdt* tabla = tdt_crear("sa");
	uint8_t niggers[3];
	uint8_t valores[15];
	niggers[0] = 0;
	niggers[1] = 0;
	niggers[2] = 0;

	for(int i = 0; i<15; i++){
		valores[i] = 0;
	}
	valores [0] = 9;
	valores [1] = 2;

	tdt_agregar(tabla,niggers,valores);

	tdt_destruir(&(tabla));

	return 0;
}
