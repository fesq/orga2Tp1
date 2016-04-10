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



void tdt_imprimirTraducciones(tdt* tabla, FILE *pFile) {
	fputs("- ", pFile);
	fputs(tabla->identificacion, pFile);
	fputs(" -\n", pFile);

	if (tabla->primera != 0) {
		for (int n1 = 0; n1 < 256; n1++) {

			if (tabla->primera->entradas[n1] != 0) {
				for (int n2 = 0; n2 < 256; n2++) {
					if (tabla->primera->entradas[n1]->entradas[n2] != 0) {
						for (int n3 = 0; n3 < 256; n3++) {
							tdtN3* nivel3 =
									tabla->primera->entradas[n1]->entradas[n2];
							if (nivel3->entradas[n3].valido) {
								uint8_t clave[3] = { n1, n2, n3 };
								char claveHex[6];
								int8Array_a_HexString(clave, 3, claveHex);
								for(int i = 0; i<6; i++) fprintf(pFile,"%c",claveHex[i]);
								fprintf(pFile," => ");

								// Cada byte del valor puede ocupar dos caracteres en hexa
								// Por lo que traducimos los 20 bytes a 30 chars
								char trad[30];
								int8Array_a_HexString(
										nivel3->entradas[n3].valor.val, 15,
										trad);
								for(int i = 0; i<30; i++) fprintf(pFile,"%c",trad[i]);
								fprintf(pFile,"\n");
							}
						}
					}
				}
			}
		}
	}
}

maxmin* tdt_obtenerMaxMin(tdt* tabla) {
	maxmin* maxMin = malloc(sizeof (maxmin));
	// Inicializamos maximos en 0, minimos en 255
	for (int i = 0; i < 15; i++) {
		if (i < 3) {
			maxMin->max_clave[i] = 0;
			maxMin->min_clave[i] = 255;
		}
		maxMin->max_valor[i] = 0;
		maxMin->min_valor[i] = 255;
	}

	if (tabla->primera != 0) {

		for (int n1 = 0; n1 < 256; n1++) {
			if (tabla->primera->entradas[n1] != 0) {

				for (int n2 = 0; n2 < 256; n2++) {
					if (tabla->primera->entradas[n1]->entradas[n2] != 0) {

						for (int n3 = 0; n3 < 256; n3++) {
							tdtN3* nivel3 =
									tabla->primera->entradas[n1]->entradas[n2];

							if (nivel3->entradas[n3].valido) {
								uint8_t clave[3] = { n1, n2, n3 };
								uint8_t* valor = nivel3->entradas[n3].valor.val;
								if (mayorLexicografico(clave, maxMin->max_clave,
										3)) {
									memcpy(maxMin->max_clave, clave, 3);
								}

								if (mayorLexicografico(maxMin->min_clave, clave,
										3)) {
									memcpy(maxMin->min_clave, clave, 3);
								}

								if (mayorLexicografico(valor, maxMin->max_valor,
										15)) {
									memcpy(maxMin->max_valor, valor, 15);
								}

								if (mayorLexicografico(maxMin->min_valor, valor,
										15)) {
									memcpy(maxMin->min_valor, valor, 15);
								}
							}
						}
					}
				}
			}
		}
	}
	return maxMin;
}


