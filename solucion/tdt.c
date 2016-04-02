#include "tdt.h"
#include "auxiliares.h"

void tdt_agregar(tdt* tabla, uint8_t* clave, uint8_t* valor) {

	// Crear si no existe el nivel 1
	if (tabla->primera == 0) {
		tabla->primera = (tdtN1 *) malloc(sizeof(tdtN1));
	}

	// Crear si no existe el nivel 2
	if (tabla->primera->entradas[*clave] == 0) {
		tabla->primera->entradas[*clave] = (tdtN2 *) malloc(sizeof(tdtN2));
	}
	tdtN2* nivel2 = tabla->primera->entradas[*clave];

	// Crear si no existe el nivel 3
	if (nivel2->entradas[*(clave + 1)] == 0) {
		nivel2->entradas[*(clave + 1)] = (tdtN3 *) malloc(sizeof(tdtN3));
	}

	tdtN3* nivel3 = nivel2->entradas[*(clave + 1)];
	// Copiar byte x byte
	for (int i = 0; i < 15; i++) {
		nivel3->entradas[*(clave + 2)].valor.val[i] = *(valor + i);
	}
	nivel3->entradas[*(clave + 2)].valido = 1;
}

void tdt_borrar(tdt* tabla, uint8_t* clave) {
	uint8_t n1, n2, n3;
	n1 = *clave;
	n2 = *(clave + 1);
	n3 = *(clave + 2);

	// Si no existe alguna tabla, retornar: no existe la clave
	if (tabla->primera == 0) {
		return;
	}
	if (tabla->primera->entradas[n1] == 0) {
		return;
	}
	tdtN2* nivel2 = tabla->primera->entradas[n1];

	if (nivel2->entradas[n2] == 0) {
		return;
	}
	tdtN3* nivel3 = nivel2->entradas[n2];

	// Si no es valido, no existe
	if (nivel3->entradas[n3].valido == 0) {
		return;
	}

	// Llegado este punto, la clave existe => la borramos.
	nivel3->entradas[*(clave + 2)].valido = 0;
	free(nivel3->entradas[*(clave + 2)].valor.val);

	//Borramos las tablas vacias
	clearEmptyTdt(tabla);
	tabla->cantidad--;

}

void tdt_imprimirTraducciones(tdt* tabla, FILE *pFile) {
	fputs(" - ", pFile);
	fputs(tabla->identificacion, pFile);
	fputs(" - \n", pFile);

	if (tabla->primera != 0) {
		for (int n1 = 0; n1 < 50; n1++) {

			if (tabla->primera->entradas[n1] != 0) {
				char claveN1[2];
				sprintf(claveN1, "%02X", n1);

				for (int n2 = 0; n2 < 50; n2++) {

					if (tabla->primera->entradas[n1]->entradas[n2] != 0) {
						char claveN2[2];
						sprintf(claveN2, "%02X", n2);

						for (int n3 = 0; n3 < 50; n3++) {
							tdtN3* nivel3 =
									tabla->primera->entradas[n1]->entradas[n2];
							if (nivel3->entradas[n3].valido) {
								char claveN3[2];
								sprintf(claveN3, "%02X", n3);
								fputs(claveN1, pFile);
								fputs(claveN2, pFile);
								fputs(claveN3, pFile);
								fputs(" => ", pFile);

								char trad[30];
								for (int i = 0; i < 15; i++) {
									char nibble[2];
									sprintf(nibble, "%02X", nivel3->entradas[n3].valor.val[i]);
									trad[i*2] = nibble[0];
									trad[i*2 + 1] = nibble[1];
								}

								fputs(trad, pFile);
								fputs("\n", pFile);
							}
						}
					}
				}
			}
		}
	}

}

maxmin* tdt_obtenerMaxMin(tdt* tabla) {
	return 0;
}

uint8_t clearEmptyN3(tdtN3* nivel3) {
	uint8_t isEmpty = 1;
	// Revisamos si quedo alguna definicion válida en nivel3
	for (int i = 0; i < 256; i++) {
		if (nivel3->entradas[i].valido) {
			isEmpty = 0;
		}
	}

	return isEmpty;
}

uint8_t clearEmptyN2(tdtN2* nivel2) {
	uint8_t isEmpty = 1;
	for (int i = 0; i < 256; i++) {
		if (!clearEmptyN3(nivel2->entradas[i])) {
			isEmpty = 0;
		} else {
			free(nivel2->entradas[i]);
		}
	}
	return isEmpty;
}

uint8_t clearEmptyN1(tdtN1* nivel1) {
	uint8_t isEmpty = 1;
	for (int i = 0; i < 256; i++) {
		if (!clearEmptyN2(nivel1->entradas[i])) {
			isEmpty = 0;
		} else {
			free(nivel1->entradas[i]);
		}
	}
	return isEmpty;
}

uint8_t clearEmptyTdt(tdt* tabla) {
	if (clearEmptyN1(tabla->primera)) {
		free(tabla->primera);
		return 1;
	} else {
		return 0;
	}
	return 0;
}
