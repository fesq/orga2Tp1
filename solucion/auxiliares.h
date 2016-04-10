#include <stdint.h>
#include "tdt.h"

#ifndef AUX_H
#define AUX_H

uint8_t clearEmptyN3(tdtN3* nivel3);
uint8_t clearEmptyN2(tdtN2* nivel2);
uint8_t clearEmptyN1(tdtN1* nivel1);
void clearEmptyTdt(tdt* tabla);

void int8Array_a_HexString(uint8_t* arrayInt8, int size, char* hexString);
void hexString_a_int8Array(char* hexString, int size, uint8_t* arrayInt8);
int mayorLexicografico(uint8_t* a, uint8_t* b, int size);
void printInt8Array(uint8_t* array, int size);

#endif
