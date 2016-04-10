#include "auxiliares.h"
#include <stdlib.h>

void hexString_a_int8Array(char* hexString, int size, uint8_t* arrayInt8){
	for(int i = 0; i< size; i=i+2){
		char iesimoByte[3] = {hexString[i],hexString[i+1], '\0'};
		arrayInt8[i/2] = strtol(iesimoByte, NULL, 16);
	}
}

/**
 * Devuelve 1 cuando a > b en orden lexicografico
 */
int mayorLexicografico(uint8_t* a, uint8_t* b, int size) {
	int esMayor = 0;
	int i = 0;
	while (!esMayor && i < size) {
		if (a[i] < b[i]) {
			// Si a es menor que b, early exit
			break;
		} else if (a[i] > b[i]) {
			esMayor = 1;
		}
		// Sino son iguales, sigue el ciclo
		// hasta encontrar un digito distinto
		// Si no se encontro uno y se llego al final
		// son iguales y esMayor sigue siendo 0
		i++;
	}

	return esMayor;
}



void printInt8Array(uint8_t* array, int size) {
	char* buffer = malloc(2 * size * sizeof(char));
	int8Array_a_HexString(array, size, buffer);
	for (int i = 0; i < 2 * size; i++) {
		printf("%c", buffer[i]);
	}
	free(buffer);
}


void int8Array_a_HexString(uint8_t* arrayInt8, int size, char* hexString) {
	int hexIndex = 0;
	for (int i = 0; i < size; i++) {
		// Cada byte se traduce a 2 chars en hexa
		char iesimoByte[2];
		sprintf(iesimoByte, "%02X", *(arrayInt8 + i));
		for (int j = 0; j < 2; j++) {
			hexString[hexIndex] = iesimoByte[j];
			hexIndex++;
		}
	}
}

/**
 * Devuelve 1 cuando la tabla de nivel 3 esta vacia
 */
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

/**
 * Devuelve 1 cuando la tabla de nivel 2 esta vacia.
 * De tener alguna referencia a una tabla de nivel 3 vacia,
 * la libera.
 */
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

/**
 * Devuelve 1 cuando la tabla de nivel 1 esta vacia.
 * De tener alguna referencia a una tabla de nivel 2 vacia,
 * la libera.
 */
uint8_t clearEmptyN1(tdtN1* nivel1) {
	uint8_t isEmpty = 1;
	for (int i = 0; i < 256; i++) {
		if (nivel1->entradas[i] != 0){
			if (!clearEmptyN2(nivel1->entradas[i])) {
				isEmpty = 0;
			} else {
				free(nivel1->entradas[i]);
				nivel1->entradas[i] = 0;
			}
		}
	}
	return isEmpty;
}

/**
 * Si la tabla de primer nivel esta vacia, la libera.
 */
void clearEmptyTdt(tdt* tabla) {
	if (clearEmptyN1(tabla->primera)) {
		free(tabla->primera);
		tabla->primera = 0;
	}
}
