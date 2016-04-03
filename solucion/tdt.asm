; FUNCIONES de C
  extern malloc
  extern free
  extern strcpy
  extern tdt_agregar
  extern tdt_borrar
  
; FUNCIONES
  global tdt_crear
  global tdt_recrear
  global tdt_cantidad
  global tdt_agregarBloque
  global tdt_agregarBloques
  global tdt_borrarBloque
  global tdt_borrarBloques
  global tdt_traducir
  global tdt_traducirBloque
  global tdt_traducirBloques
  global tdt_destruir

; /** defines offsets y size **/
  %define TDT_OFFSET_IDENTIFICACION   0
  %define TDT_OFFSET_PRIMERA          8
  %define TDT_OFFSET_CANTIDAD        16
  %define TDT_SIZE                   20

  %define BLOQUE_SIZE                18
  %define BLOQUE_OFFSET_CLAVE        0
  %define BLOQUE_OFFSET_VALOR        3

  %define NULL			     0
  
section .text

; =====================================
; tdt* tdt_crear(char* identificacion)
tdt_crear:
; Guardamos la identificacion en la pila, alineandola
PUSH RDI 

; Pedimos un bloque de 20 bytes
MOV RDI, TDT_SIZE 
CALL malloc

; Ponemos la identificacion en R8, restaurando la pila
; restaurando la pila a su estado original
POP R8

; Ahora tenemos en RAX un puntero al inicio del bloque de 20 bytes

; En los primeros 8 ponemos la dirección de la id
MOV [RAX + TDT_OFFSET_IDENTIFICACION], R8

; En los siguientes 8 bytes ponemos 0 para indicar un punter nulo a la primer tabla
MOV qword [RAX + TDT_OFFSET_PRIMERA], 0

; En los ultimos cuatro byes ponemos un 0, ya que la tabla esta vacia
MOV dword [RAX + TDT_OFFSET_CANTIDAD], 0 

; El puntero al struct ya esta en RAX, asi que solo retornamos.
; Como no tocamos ni pila ni registros a preservar, cumple convencion C.
RET

; =====================================
; void tdt_recrear(tdt** tabla, char* identificacion)
tdt_recrear:

; =====================================
; uint32_t tdt_cantidad(tdt* tabla)
tdt_cantidad:
; En RDI tenemos el puntero a la tabla
MOV RAX, 0
MOV dword EAX, [RDI + TDT_OFFSET_CANTIDAD]
RET


; =====================================
; void tdt_agregarBloque(tdt* tabla, bloque* b)
tdt_agregarBloque:
JMP tdt_agregarBloques

; =====================================
; void tdt_agregarBloques(tdt* tabla, bloque** b)
tdt_agregarBloques:
; En RDI tenemos la tabla, RSI el bloque
PUSH RBP ; Alineamos la pila
.agregar:
CMP RSI, 0 ;cuando el puntero de bloques sea 0, terminamos (null-terminate)
JE .done
     ; Por cada bloque vamos a llamar a tdt_agregar, con la signatura:
     ; void tdt agregar(tdt* tabla, uint8 t* clave, uint8 t* valor)
     ; Por lo que: RDI <- tabla, RSI <- clave, RDX <- valor
     ; Ya tenemos tabla y clave en sus registros, agregamos puntero a valor
     MOV RDX, RSI
     ADD RDX, BLOQUE_OFFSET_VALOR
     ; Guardamos las posiciones del bloque y tabla 
     PUSH RSI
     PUSH RDI
     
     CALL tdt_agregar
     
     POP RDI
     POP RSI
     
     ADD RSI, BLOQUE_SIZE
     JMP .agregar
.done:
POP RBP
RET
        
; =====================================
; void tdt_borrarBloque(tdt* tabla, bloque* b)
tdt_borrarBloque:
        
; =====================================
; void tdt_borrarBloques(tdt* tabla, bloque** b)
tdt_borrarBloques:
        
; =====================================
; void tdt_traducir(tdt* tabla, uint8_t* clave, uint8_t* valor)
tdt_traducir:
; En RDI : tabla, RSI : clave (3bytes), RDX : valor(15bytes)
PUSH RBP

MOV RAX, [RDI + TDT_OFFSET_PRIMERA]
CMP RAX, 0
JE .done ; Si no esta la primer tabla, early exit
;Seguimos: RAX apunta a la primer tabla
MOV R8, 0; R8 sera el primer id de la clave
MOV R9, 0; R9 el segundo
MOV R10, 0; R10 el tercero
MOV byte R8L, [RSI]
MOV byte R9L, [RSI+1]
MOV byte R10L, [RSI+2]
; R8 lo multiplicamos por el tamaño de las entradas
; que al ser punteros, es 8
MOV RAX, [RAX] ; Ahora RAX apunta a una tabla de N1
LEA RAX, [RAX + R8 * 8] ; Ahora RAX esta en la posicion de la clave N1
CMP RAX, 0
JE .done ; Si no hay tabla de N2, early exit

MOV RAX, [RAX] ; Ahora RAX apunta a una tabla de N2
LEA RAX, [RAX + R9 * 8] ; Ahora RAX esta en la posicion de la clave N2
CMP RAX, 0
JE .done ; Si no hay tabla de N3, early exit

MOV RAX, [RAX] ; Ahora RAX apunta a una tabla de N3
LEA RAX, [RAX + R10 * 16] ; Ahora RAX esta en la posicion de la clave N3

; Si el ultimo Byte es 0, no es un valor valido
MOV RCX, 0
MOV byte CL, [RAX + 15]
CMP CL,0
JE .done

; Sino, copiamos los primeros 15 bytes al valor parametro
MOV RCX,0
.copiar:
MOV R8, [RAX + RCX]
MOV [RDX + RCX], R8
INC RCX
CMP RCX, 15
JNE .copiar

.done:
POP RBP
RET
        
; =====================================
; void tdt_traducirBloque(tdt* tabla, bloque* b)
tdt_traducirBloque:
JMP tdt_traducirBloques

; =====================================
; void tdt_traducirBloques(tdt* tabla, bloque** b)
tdt_traducirBloques:
; En RDI : tabla, RSI : Arreglo de bloques
PUSH RBP ; Alineamos la pila
.traducir:
MOV R8, [RSI]
CMP R8, 0 ;cuando el puntero de bloques sea 0, terminamos (null-terminate)
JE .done
	; RSI apunta al inicio del bloque, que dada su disposicion 
	; en memoria, es equivalente a apuntar a la clave
	MOV RDX, [R8]
	ADD RDX, BLOQUE_OFFSET_VALOR
	; Guardamos los punteros a tabla y array de bloques
	; para preservarlos despues de llamar a traducir	
	PUSH RDI
	PUSH RSI
	; En RDI tenemos la tabla, RSI la clave y en RDX el valor
	CALL tdt_traducir
	
	POP RSI
	POP RDI
	
	ADD RSI, BLOQUE_SIZE
    	JMP .traducir
.done:
POP RBP
RET
        
; =====================================
; void tdt_destruir(tdt** tabla)
tdt_destruir:
PUSH RBP ; <- Alineada
PUSH R12
PUSH R13 ; <- Alineada
PUSH R14
PUSH R15 ; <- Alineada
; En RDI: Puntero a array de tablas
; Pila alineada
MOV R15, RDI

.otraTabla:
CMP R15, NULL
JE .done
MOV R13, [R15] ; R13 ahora apunta a una tdt
MOV R13, [R13+TDT_OFFSET_PRIMERA] ;Ponemos en R13 la tabla N1 con la que vamos a trabajar
CMP R13, NULL ;Si no tiene nivel 1, solo hay que borrar el "encabezado" (el tdt)
JE .destruirTDT
; Si seguimos aca, hay que destruir el nivel 1
; R13 tiene un puntero a la primer tabla, la recorremos:
	MOV R14, 0 ; R14 es nuestro indice de N1
	.cicloN1:
	MOV R12, [R13 + R14 * 8] ; R12 es nuestro cursor
	CMP R12, NULL 
	JE .stepN1 ; Si la posicion N1[R14] esta vacia, seguimos de largo
	; Sino, Borramos el nivel 2 en esa posicion. R12 es el puntero a la tabla de nivel 2

		MOV R10, 0; R10 es nuestro indicie de N2
		.cicloN2:
		MOV R11, [R12 + R10 * 8]
		CMP R11, NULL
		JE .stepN2
		; Si no saltamos, en R11 tenemos un puntero a una tabla de nivel 3
		; Esta no tiene mas punteros que liberar, por lo que la liberamos
		PUSH R11 ; Pusheamos los registros que no se conservan por convencion
		PUSH R10 ; Como son 2 y la pila estaba alineada, sigue alineada
		MOV RDI, R11
		CALL free ; Liberamos una tdtN3
		POP R10
		POP R11
		.stepN2:
		INC R10
		CMP R10, 256
		JNE .cicloN2
		
	; Ya borramos todos los registros de la tabla N2 que habia en R12 
	; Solo falta liberarla. La pila esta alineada porque al sacar R10 y 11
	; volvio a la alineacion inicial del stack-frame que preparamos.
	MOV RDI, R12
	CALL free ; Liberamos una tdtN2

	.stepN1:
	INC R14
	CMP R14, 256
	JNE .cicloN1

.destruirTDT:
MOV RDI, R13 ; Liberamos una tdtN1
CALL free
MOV RDI, [R15]
CALL free ; Liberamos una tdt
ADD R15, 8

.done:
POP R15
POP R14
POP R13
POP R12
POP RBP
RET




