#include "tdt.h"
#include "auxiliares.h"

void tdt_agregar(tdt* tabla, uint8_t* clave, uint8_t* valor) {

	// Crear si no existe el nivel 1
	if (tabla->primera == 0) {
		tabla->primera = (tdtN1 *) malloc(sizeof(tdtN1));
		// Inicializamos todas las entradas a 0
		for (int i = 0; i < 256; i++) {
			tabla->primera->entradas[i] = 0;
		}
	}

	// Crear si no existe el nivel 2
	if (tabla->primera->entradas[*clave] == 0) {
		tabla->primera->entradas[*clave] = (tdtN2 *) malloc(sizeof(tdtN2));
		// Inicializamos todas las entradas a 0
		for (int i = 0; i < 256; i++) {
			tabla->primera->entradas[*clave]->entradas[i] = 0;
		}
	}
	tdtN2* nivel2 = tabla->primera->entradas[*clave];

	// Crear si no existe el nivel 3
	if (nivel2->entradas[*(clave + 1)] == 0) {
		nivel2->entradas[*(clave + 1)] = (tdtN3 *) malloc(sizeof(tdtN3));
		// Inicializamos todas las entradas a invalidas
		for (int i = 0; i < 256; i++) {
			nivel2->entradas[*(clave + 1)]->entradas[i].valido = 0;
		}
	}

	tdtN3* nivel3 = nivel2->entradas[*(clave + 1)];
	// Copiar byte x byte
	for (int i = 0; i < 15; i++) {
		nivel3->entradas[*(clave + 2)].valor.val[i] = *(valor + i);
	}
	nivel3->entradas[*(clave + 2)].valido = 1;
	tabla->cantidad++;
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
	//free(nivel3->entradas[*(clave + 2)].valor.val);

	//Borramos las tablas vacias
	clearEmptyTdt(tabla);
	tabla->cantidad--;

}

void int8Array_a_HexString(uint8_t* arrayInt8, int largo, char* hexString) {
	int hexIndex = 0;
	for (int i = 0; i < largo; i++) {
		char claveNi[2];
		sprintf(claveNi, "%02X", *(arrayInt8 + i));
		for (int j = 0; j < 2; j++) {
			hexString[hexIndex] = claveNi[j];
			hexIndex++;
		}
	}
}

void tdt_imprimirTraducciones(tdt* tabla, FILE *pFile) {
	fputs(" - ", pFile);
	fputs(tabla->identificacion, pFile);
	fputs(" - \n", pFile);

	if (tabla->primera != 0) {
		for (uint8_t n1 = 0; n1 < 255; n1++) {

			if (tabla->primera->entradas[n1] != 0) {
				for (uint8_t n2 = 0; n2 < 255; n2++) {
					if (tabla->primera->entradas[n1]->entradas[n2] != 0) {
						for (uint8_t n3 = 0; n3 < 255; n3++) {
							tdtN3* nivel3 =
									tabla->primera->entradas[n1]->entradas[n2];
							if (nivel3->entradas[n3].valido) {
								uint8_t clave[3] = { n1, n2, n3 };
								char claveHex[6];
								int8Array_a_HexString(clave, 3, claveHex);

								fputs(claveHex, pFile);
								fputs(" => ", pFile);

								// Cada byte del valor puede ocupar dos caracteres en hexa
								// Por lo que traducimos los 20 bytes a 30 chars
								char trad[30];
								int8Array_a_HexString(
										nivel3->entradas[n3].valor.val, 15,
										trad);
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
		if (nivel2->entradas[i] != 0) {
			if (!clearEmptyN3(nivel2->entradas[i])) {
				isEmpty = 0;
			} else {
				free(nivel2->entradas[i]);
				nivel2->entradas[i] = 0;
			}
		}
	}
	return isEmpty;
}

uint8_t clearEmptyN1(tdtN1* nivel1) {
	uint8_t isEmpty = 1;
	for (int i = 0; i < 256; i++) {
		if (nivel1->entradas[i] != 0)
			if (!clearEmptyN2(nivel1->entradas[i])) {
				isEmpty = 0;
			} else {
				free(nivel1->entradas[i]);
				nivel1->entradas[i] = 0;
			}
	}
	return isEmpty;
}

uint8_t clearEmptyTdt(tdt* tabla) {
	if (clearEmptyN1(tabla->primera)) {
		free(tabla->primera);
		tabla->primera = 0;
		return 1;
	} else {
		return 0;
	}
	return 0;
}
