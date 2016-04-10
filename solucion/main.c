#include <stdio.h>
#include "tdt.h"
#include "auxiliares.h"

void printMaxMin(tdt* tabla);
bloque* crearBloque(char* clave, char* valor);


char *archivoCasoChico  =  "salida.caso.chico.txt";
char *archivoCasoGrande =  "salida.caso.grande.txt";
uint8_t clave1[3] = {1,2,3};
uint8_t valor1[15] = {3,4,5,6,7,8,4,5,63,2,3,4,5,6,5};
uint8_t clave2[3] = {2,2,2};
uint8_t valor2[15] = {3,4,5,3,7,8,4,5,63,2,3,4,5,6,5};
uint8_t clave3[3] = {3,2,3};
uint8_t valor3[15] = {3,4,5,6,7,8,4,5,0,2,3,4,5,6,5};
uint8_t clave4[3] = {8,9,3};
uint8_t valor4[15] = {3,4,5,6,7,8,4,5,63,2,3,4,5,6,5};
uint8_t clave5[3] = {0,2,2};
uint8_t valor5[15] = {63,4,5,2,7,8,4,5,0,2,3,4,5,6,5};
uint8_t clave6[3] = {7,2,3};
uint8_t valor6[15] = {3,4,5,6,7,8,4,5,63,2,3,4,5,6,5};
uint8_t clave7[3] = {8,2,3};
uint8_t valor7[15] = {9,4,5,6,7,8,4,5,63,2,3,4,5,6,5};

uint8_t claveMin[3] = {0,0,0};
uint8_t claveMax[3] = {255,255,255};
uint8_t valorMax[15] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
uint8_t valorMin[15] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

bloque b1 = {{1,5,3},{3,4,5,6,7,8,4,5,6,2,3,4,5,0,5}};
bloque b2 = {{2,5,2},{3,4,5,3,7,8,4,3,0,2,0,4,5,6,5}};
bloque b3 = {{3,5,3},{3,4,5,6,7,8,4,5,0,2,3,4,5,6,5}};

bloque* b[4] = {&b1,&b2,&b3,0};
void printmaxmin(FILE *pFile, tdt* tabla) {
    int i;
    maxmin *mm = tdt_obtenerMaxMin(tabla);
    fprintf(pFile,"max_clave = %i",mm->max_clave[0]);
    for(i=1;i<3;i++) fprintf(pFile,"-%i",mm->max_clave[i]);
    fprintf(pFile,"\n");
    fprintf(pFile,"min_clave = %i",mm->min_clave[0]);
    for(i=1;i<3;i++) fprintf(pFile,"-%i",mm->min_clave[i]);
    fprintf(pFile,"\n");
    fprintf(pFile,"max_valor = %i",mm->max_valor[0]);
    for(i=1;i<15;i++) fprintf(pFile,"-%i",mm->max_valor[i]);
    fprintf(pFile,"\n");
    fprintf(pFile,"min_valor = %i",mm->min_valor[0]);
    for(i=1;i<15;i++) fprintf(pFile,"-%i",mm->min_valor[i]);
    fprintf(pFile,"\n");
    free(mm);
}

void printBloque(FILE *pFile, bloque* b) {
    int i;
    fprintf(pFile,"%i",b->clave[0]);
    for(i=1;i<3;i++) fprintf(pFile,"-%i",b->clave[i]);
    fprintf(pFile," => ");
    fprintf(pFile,"%i",b->valor[0]);
    for(i=1;i<15;i++) fprintf(pFile,"-%i",b->valor[i]);
    fprintf(pFile,"\n");
}

int main3(void) {
	   FILE *pFile;
	      pFile = fopen( archivoCasoGrande, "a" );

	      /* creo un vector de bloques con datos al azar */
	      uint32_t i,j,n = 100;
	      bloque* arrayBloques = (bloque*)malloc(sizeof(bloque)*(n));
	      bloque** bb = (bloque**)malloc(sizeof(bloque*)*(n+1));
	      bloque* arrayBloquesTraducir = (bloque*)malloc(sizeof(bloque)*(n));
	      bloque** bbTraducir = (bloque**)malloc(sizeof(bloque*)*(n+1));
	      uint32_t *fastFill = (uint32_t*)(arrayBloques);
	      for(i=0;i<n*sizeof(bloque)/sizeof(uint32_t);i++)
	        fastFill[i] = rand();
	      for(i=0;i<n;i++) {
	        bb[i] = &(arrayBloques[i]);
	        bbTraducir[i] = &(arrayBloquesTraducir[i]);
	        bbTraducir[i]->clave[0] = bb[i]->clave[0];
	        bbTraducir[i]->clave[1] = bb[i]->clave[1];
	        bbTraducir[i]->clave[2] = bb[i]->clave[2];
	        for(j=0;j<15;j++)
	          bbTraducir[i]->valor[j] = 0;
	      }
	      bb[n]=0;
	      bbTraducir[n]=0;

	      /* comienzo de casos */
	      tdt *tabla = tdt_crear("gande!");

	      fputs( ">>> Test : agregar en bloque\n", pFile );

	      tdt_agregarBloques(tabla,bb);
	      tdt_imprimirTraducciones(tabla, pFile);
	      fprintf( pFile, "%i\n", tdt_cantidad(tabla) );
	      fprintf(pFile,"\n");

	      fputs( ">>> Test : borrar en bloque\n", pFile );

	      tdt_borrarBloques(tabla,&(bb[n/2]));
	      tdt_imprimirTraducciones(tabla, pFile);
	      fprintf( pFile, "%i\n", tdt_cantidad(tabla) );
	      fprintf(pFile,"\n");

	      fputs( ">>> Test : borrar bloques\n", pFile );

	      for(i=0;i<n/4;i++)
	        tdt_borrarBloque(tabla,bb[i]);
	      tdt_imprimirTraducciones(tabla, pFile);
	      fprintf( pFile, "%i\n", tdt_cantidad(tabla) );

	      fputs( ">>> Test : borrar individual\n", pFile );

	      for(i=n/4;i<n/2;i++)
	        tdt_borrar(tabla,bb[i]->clave);
	      tdt_imprimirTraducciones(tabla, pFile);
	      fprintf( pFile, "%i\n", tdt_cantidad(tabla) );
	      fprintf(pFile,"\n");

	      fputs( ">>> Test : borrar y agregar aleatoriamente paso 1\n", pFile );

	            for(i=0;i<n;i++) {
	              tdt_agregarBloque(tabla,bb[rand()%n]);
	              tdt_agregarBloque(tabla,bb[rand()%n]);
	              tdt_borrar(tabla,bb[rand()%n]->clave);
	            }
	            tdt_imprimirTraducciones(tabla, pFile);
	            printmaxmin(pFile, tabla);
	            fprintf( pFile, "%i\n", tdt_cantidad(tabla) );
	            fprintf(pFile,"\n");

	            fputs( ">>> Test : borrar y agregar aleatoriamente paso 2\n", pFile );

	            for(i=0;i<n;i++) {
	              tdt_agregarBloque(tabla,bb[rand()%n]);
	              tdt_borrar(tabla,bb[rand()%n]->clave);
	              tdt_borrar(tabla,bb[rand()%n]->clave);
	              tdt_borrar(tabla,bb[rand()%n]->clave);
	              tdt_borrar(tabla,bb[rand()%n]->clave);
	            }
	            tdt_imprimirTraducciones(tabla, pFile);
	            printmaxmin(pFile, tabla);
	            fprintf( pFile, "%i\n", tdt_cantidad(tabla) );
	            fprintf(pFile,"\n");

	            fputs( ">>> Test : maximos y minimos\n", pFile );
	            tdt_agregar(tabla,clave1,valorMax);
	            printmaxmin(pFile, tabla);
	            tdt_agregar(tabla,clave2,valorMin);
	            printmaxmin(pFile, tabla);
	            tdt_agregar(tabla,claveMax,valor1);
	            printmaxmin(pFile, tabla);
	            tdt_agregar(tabla,claveMin,valor2);
	            printmaxmin(pFile, tabla);
	            fprintf(pFile,"\n");

	            fputs( ">>> Test : traducir bloques\n", pFile );

	            tdt_traducirBloques(tabla,bbTraducir);
	            fprintf( pFile, "%i\n", tdt_cantidad(tabla) );
	            for(i=0;i<n;i++)
	              printBloque(pFile,bbTraducir[i]);
	            fprintf(pFile,"\n");

	            fputs( ">>> Test : traducir en bloque\n", pFile );

	            for(i=0;i<n;i++)
	              for(j=0;j<15;j++)
	                arrayBloquesTraducir[i].valor[j] = 0;
	            for(i=0;i<n;i++)
	              tdt_traducirBloque(tabla,&(arrayBloquesTraducir[i]));
	            for(i=0;i<n;i++)
	              printBloque(pFile,bbTraducir[i]);
	            fprintf(pFile,"\n");

	            fputs( ">>> Test : traducir bloques\n", pFile );

	            for(i=0;i<n;i++)
	              for(j=0;j<15;j++)
	                arrayBloquesTraducir[i].valor[j] = 0;
	            for(i=0;i<n;i++)
	              tdt_traducir(tabla,(uint8_t*)&(arrayBloquesTraducir[i].clave),(uint8_t*)&(arrayBloquesTraducir[i].valor));
	            for(i=0;i<n;i++)
	              printBloque(pFile,bbTraducir[i]);
	            fprintf(pFile,"\n");

	      tdt_destruir(&tabla);

	      free(bb);
	      free(arrayBloques);
	      free(bbTraducir);
	      free(arrayBloquesTraducir);
	      fclose( pFile );

	      return 0;
}

int main(void) {

	tdt* tabla = tdt_crear("pepe");
	uint8_t claveB[3] = { 255, 255, 255 };
	uint8_t valorB[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	uint8_t claveA[3] = { 0, 0, 0 };
	uint8_t valorA[15] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255 };

	//uint8_t valorC[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// Agregamos 00 00 00 : FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
	tdt_agregar(tabla, claveA, valorA);

	// Agregamos FF FF FF : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	tdt_agregar(tabla, claveB, valorB);

	printMaxMin(tabla);

	tdt_recrear(&(tabla),"Pepe");

	bloque** bloques = malloc(5 * sizeof(bloque*));
	bloques[0] = crearBloque("050505","123456789ABCDEF123456789ABCDEF");
	bloques[1] = crearBloque("FFFFFF","112233445566778899AABBCCDDEEFF");
	bloques[2] = crearBloque("53FFAA","111222333444555666777888999AAA");
	bloques[3] = crearBloque("10EE05","111122223333444455556666777788");
	bloques[4] = 0;

	tdt_agregarBloques(tabla, bloques);

	for(int i = 0; i<4; i++){
		free(bloques[i]);
	}
	free(bloques);

	tdt_traducir(tabla,claveA,valorA);
	bloque* b = crearBloque("050505","000000000000000000000000000000");
	tdt_traducirBloque(tabla,b);
	free(b);

	bloques = malloc(3 * sizeof(bloque*));
	bloques[0] = crearBloque("FFFFFF","112233445566778899AABBCCDDEEFF");
	bloques[1] = crearBloque("53FFAA","111222333444555666777888999AAA");
	bloques[2] = 0;

	tdt_borrarBloques(tabla, bloques);

	for(int i = 0; i<2; i++){
		free(bloques[i]);
	}
	free(bloques);


	bloques = malloc(2 * sizeof(bloque*));
	bloques[0] = crearBloque("050505","000000000000000000000000000000");
	//bloques[1] = crearBloque("10EE05","000000000000000000000000000000");
	bloques[1] = 0;


//	tdt_traducirBloques(tabla, bloques);
	printInt8Array(bloques[0]->valor,15);
	printf("\n");
	//printInt8Array(bloques[1]->valor,15);
	//printf("\n");
	free(bloques[0]);
	/*for(int i = 0; i<2; i++){
		free(bloques[i]);
	}*/
	free(bloques);


	printMaxMin(tabla);

	FILE* file = fopen("traducciones.txt", "w+");
	tdt_imprimirTraducciones(tabla, file);
	fclose(file);

	printf("\nCantidad: %d", tdt_cantidad(tabla));

	tdt_borrar(tabla, claveB);

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

bloque* crearBloque(char* clave, char* valor){
	bloque* bloq = (bloque*)malloc(sizeof(bloque) * 1);
	uint8_t claveBloque[3];
	hexString_a_int8Array(clave,6,claveBloque);
	memcpy(bloq->clave,claveBloque,3);
	uint8_t valorBloque[15];
	hexString_a_int8Array(valor,30,valorBloque);
	memcpy(bloq->valor,valorBloque,15);
	return bloq;
}
