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

section .text

; =====================================
; tdt* tdt_crear(char* identificacion)
tdt_crear:
; Guardamos la identificacion
MOV R8, RDI 

; Pedimos un bloque de 20 bytes
MOV RDI, TDT_SIZE 
CALL malloc

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

; =====================================
; void tdt_agregarBloques(tdt* tabla, bloque** b)
tdt_agregarBloques:
; En RDI tenemos la tabla, RSI el bloque
CMP RSI, 0 ;cuando el puntero de bloques sea 0, terminamos (null-terminate)
JE .done
     ; Por cada bloque vamos a llamar a tdt_agregar, con la signatura:
     ; void tdt agregar(tdt* tabla, uint8 t* clave, uint8 t* valor)
     ; Por lo que: RDI <- tabla, RSI <- clave, RDX <- valor
     ; Ya tenemos tabla y clave en sus registros, agregamos puntero a valor
     MOV RDX, RSI
     ADD RDX, BLOQUE_OFFSET_VALOR 
     CALL tdt_agregar
     ADD RSI, BLOQUE_SIZE
     JMP tdt_agregarBloques
   .done:
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
        
; =====================================
; void tdt_traducirBloque(tdt* tabla, bloque* b)
tdt_traducirBloque:

; =====================================
; void tdt_traducirBloques(tdt* tabla, bloque** b)
tdt_traducirBloques:
        
; =====================================
; void tdt_destruir(tdt** tabla)
tdt_destruir:
PUSH RBP
PUSH R12
PUSH R13
PUSH R14
PUSH R15
; En RDI: Puntero a array de tablas
; Pila alineada
MOV R15, RDI

.otraTabla:
CMP R15, NULL
JE .done
MOV R13, [RDI+TDT_OFFSET_PRIMERA] ;Ponemos en R13 la tabla N1 con la que vamos a trabajar
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
		CALL free		
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
	CALL free

	.stepN1:
	INC R14
	CMP R14, 256
	JNE .cicloN1

.destruirTDT:
LEA RDI, [R15+TDT_OFFSET_IDENTIFICACION]
CALL free
MOV RDI, R15
CALL free
ADD R15, TDT_SIZE

.done:
POP R15
POP R14
POP R13
POP R12
POP RBP
RET




