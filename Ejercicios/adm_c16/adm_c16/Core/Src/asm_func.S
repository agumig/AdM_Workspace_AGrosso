.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_svc
.global asm_sum
.global asm_zeros
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12
.global asm_pack32to16
.global asm_max
.global asm_filtroVentana10
.global asm_downsampleM
.global asm_invertir
.global asm_eco

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_zeros (uint32_t * vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: dirección de memoria de inicio del vector.
@   r1: longitud del vector mencionado.
.thumb_func					@ Se indica al ensamblador que es una función.
	asm_zeros:				@ Etiqueta/nombre de la función.
		push {R2}			@ Resguardo el valor de r2
		mov R2, 0			@ r2 = 0.
		asm_zeros_bucle:	@ Etiqueta de comienzo de bucle.
			str R2, [R0]	@ Seteo en 0 el contenido de la dirección apuntada por r0.
			add R0, 4		@ Me desplazo 4 bytes en la memoria.
			subs R1, 1		@ Resto en 1 la longitud y actualizo flags del procesador.
			bne asm_zeros_bucle	@ Mientras Z != 0, se salta a la etiqueta de bucle.
		pop {R2}
		bx lr				@ Retorno a quien llamó a la función.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar);
@
@ Parametros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@	r2: longitud del vector de entrada.
@	r3: valor escalar a usar en la multiplicación.
.thumb_func
	asm_productoEscalar32:
		push {R4}				@ Guardo el registro que voy a usar
		asm_prod32_bucle:		@ Etiqueta de bucle
			ldr R4, [R0], #4	@ r4 = *r0;  r0 = r0 + 4 bytes
			mul R4, R3			@ r4 = r4 * r3
			str R4, [R1], #4	@ *r1 = r4; r1 = r1 + 4 bytes
			subs R2, 1			@ Resto en 1 la longitud y actualizo flags del procesador.
			bne asm_prod32_bucle	@ Mientras Z != 0, se salta a la etiqueta de bucle.
		pop {R4}
		bx lr		@ Retorno a quien llamó a la función.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@	r2: longitud del vector de entrada.
@	r3: valor escalar a usar en la multiplicación.
.thumb_func
	asm_productoEscalar16:
		push {R4}				@ Guardo el registro que voy a usar
		asm_prod16_bucle:		@ Etiqueta de bucle
			ldrh R4, [R0], #2	@ r4 = *r0;  r0 = r0 + 4 bytes
			mul  R4, R3			@ r4 = r4 * r3
			strh R4, [R1], #2	@ *r1 = r4; r1 = r1 + 4 bytes
			subs R2, 1			@ Resto en 1 la longitud y actualizo flags del procesador.
			bne asm_prod16_bucle	@ Mientras Z != 0, se salta a la etiqueta de bucle.
		pop {R4}
		bx lr		@ Retorno a quien llamó a la función.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@	void asm_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@	r2: longitud del vector de entrada.
@	r3: valor escalar a usar en la multiplicación.

.thumb_func
	asm_productoEscalar12:
		push {R4}				@ Guardo el registro que voy a usar

@		mrs R4, APSR			@ Bloque para limpiar el flag Q (overflow). No es necesario en este ejercicio.
@		bfc R4, 27, 1
@		msr APSR_nzcvq, R4

		asm_prod12sat_bucle:	@ Etiqueta para bucle
			ldrh R4, [R0], #2	@ Guardo la mitad de lo apuntado por r0 en r4 y me muevo 16 bits en r0
			mul R4, R3			@ Multiplico
			usat R4, #12, R4	@ Saturo (no signado) a 12 bits r4 y lo guardo en r4
			strh R4, [R1], #2	@ Guardo r4 en *r1 y muevo r1 16 bits
			subs R2, 1
			bne asm_prod12sat_bucle
		pop {R4}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@	void asm_pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@	r2: longitud del vector de entrada.
.thumb_func
	asm_pack32to16:
		push {R3}		@ Resguardo el registro
		asm_pack32to16_bucle:
			ldr R3, [R0], #4	@ Cargo el contenido en r3 y me desplazo 4 bytes
			lsr R3, R3, #16		@ Shift right de 16 bits
			strh R3, [R1], #2	@ Guardo la mitad del valor del registro y me desplazo 2 bytes
			subs R2, 1
			bne asm_pack32to16_bucle
		pop {r3}
		bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@	int32_t asm_max (int32_t * vectorIn, uint32_t longitud);
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: longitud del vector de entrada.
@ Registros a usar
@	r2: resguardo del máximo valor
@	r3: resguardo de la posición en memoria del máximo valor
@	r4: contador
@	r5: para leer la memoria
@
@ Retorno
@	r0: valor de la posición del vector donde se encuentra el valor máximo
.thumb_func
	asm_max:
		push {R2-R5}	@ Resguardo en pila los registros a usar. Al final del programa los recupero.
		mov R2, 0	@ Inicializo en cero
		mov R3, 0	@ Inicializo en cero
		mov R4, 0	@ Inicializo en cero
		mov R5, 0	@ Inicializo en cero
		asm_max_bucle:
			ldr R5, [R0], #4	@ Leo el valor actual del vector y me muevo 4 bytes en memoria hacia el próximo elemento
			cmp R5, R2			@ Comparo el valor actual (r5) con el último máximo (r2)
			ITT GT				@ Ejecución condicional de próximas dos instrucciones: si r5 es mayor a r2
				movgt R3, R4	@ Guardo el índice del vector
				movgt R2, R5	@ Guardo el nuevo valor máximo
			add R4, 1			@ Incremento el valor del índice en el vector
			subs R1, 1			@ Resto al tamaño del vector y, por lo tanto, a la cantidad de ciclos del bucle
			bne asm_max_bucle
		mov R0, R3				@ Guardo el máximo (valor de retorno)
		pop {R2-R5}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@		void asm_downsampleM (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N);
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@   r2: longitud del vector de entrada.
@	r3: longitud del paso a realizar para borrar un elemento
@ Registros a usar
@	r4: auxiliar para lectura de [r0]
@	r5: contador desde 1 a r3

.thumb_func
	asm_downsampleM:
		push {R4-R5}	@ Resguardo valores actuales
		mov R5, 0		@ Inicializo en 0
		asm_downsampleM_loop:
			add R5, 1			@ Indico número elemento a leer
			ldr R4, [R0], #4	@ Leo el elemento del vector de entrada y me desplazo una posición
			cmp R5, R3			@ Veo si es un elemento a copiar o a descartar según valor de R3
			ITE EQ				@ Si es igual...
				movEQ R5, 0			@ Si es igual, reinicio el contador
				strNE R4, [R1], #4	@ Si no es igual, copio el valor y me desplazo en el vector de salida una posición
			subs R2, 1			@ Resto en 1 el indicador de los giros del bucle y actualizo flags
			bne asm_downsampleM_loop
		pop {R4-R5}	@ Recupero valores iniciales
		bx lr		@ Vuelvo a la dirección donde se invocó a la función



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@		void asm_invertir (uint16_t * vector, uint32_t longitud);
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: longitud del vector de entrada.
@ Registros a usar
@	r2: auxiliar para lectura de [r0]
@	r3: auxiliar para 2da lectura de [r0]
@	r4: contador de pasos de a 2 bytes
@	r5: contador de pasos de a -2 bytes



.thumb_func
	asm_invertir:
		push {R2-R5}
		mov R4, 2			@ Cargo el valor 2 (2 bytes) para usarlo en la inicialización de R5
		mov R5, R1			@ Cargo la cantidad N de elementos
		sub R5, 1			@ Cargo N - 1
		mul R5, R4			@ Multiplico por la cantidad de bytes de cada elemento de los N - Obtengo los bytes a desplazarme en memoria para obtener el elementro N - 1
		mov R4, 0			@ Inicializo en 0 (primer dígito del vector)
		asm_invertir_loop:
			ldrh R2, [R0, R4]	@ Leo el elemento 0 + X
			ldrh R3, [R0, R5]	@ Leo el elemento N - X
			strh R2, [R0, R5]	@ Invierto posiciones
			strh R3, [R0, R4]	@
			add R4, 2			@ Paso al elemento siguiente
			sub R5, 2			@ Paso al elemento anterior
			cmp R4, R5			@ Comparo
			blo	asm_invertir_loop @ Si R4 es menor a R5 (no llegué al medio), entonces, continuo en el bucle
		pop {R2-R5}
		bx lr		@ Vuelvo a la dirección donde se invocó a la función


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@		void asm_eco (int16_t * vectorIn, int16_t * vectorOut, uint32_t longitud)
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@   r2: longitud del vector de entrada.
@	r3: muestra desde la cual introducir el eco
@ Registros a usar
@	r4: usado para leer [r0]
@	r5: usado para resguardar r0 y luego usado para leer [R1]
@	r6: usado para resguardar r1
@	r7: auxiliar


.thumb_func
	asm_eco:
		push {R4-R7}	@ Resguardo registros a usar
		lsr R2, 1	@ Divido por dos la longitud ya que se leerá de a dos valores de 16 bits
		lsr R3, 1	@ Divido por dos la posición inicial del eco ya que se leerá de a dos valores de 16 bits
		mov R5, 0	@ Será usado de contador
		mov R6, R2	@ Resguardo la cantidad de elementos

		asm_eco_copy_loop:			@ Loop donde copio el vector entero
			ldr R4, [R0, R5]		@ Leo R0
			str R4, [R1, R5]		@ Copio R0 en R1
			add R5, 4				@ Muevo el índice 4 bytes
			subs R2, 1
			bne asm_eco_copy_loop

		mov R2, R6	@ Recupero la cantidad de elementos del vector
		add R1, R3	@ Desplazo hasta la posición del primer elemento al que incorporarle eco
		sub R2, R3	@ Calculo la cantidad de elementos a los que incorporarles eco

		mov R7, #0x0000FFFF	@ Máscara para eliminar los más significativos previo a una suma

		asm_eco_eco_loop:
			@ldr R4, [R0], #4
			@slsr16 R4, 1		@ Divido por dos (genero el eco) ambas partes de 16 bits con SIMD

			ldrh R5, [R0], #2	@ Cargo los 16 bits menos significativos
			sxth R5, R5			@ Extiendo el signo del harlf word
			lsr R5, 1			@ Divido por dos
			mul R5, R5, R7			@ Aplico la máscara
			lsl R5, 16			@ Paso los 16 bits menos significativos a la parte alta

			ldrh R4, [R0], #2	@ Cargo 16 bits
			sxth R4, R4			@ Extiendo el signo del harlf word
			lsr R4, 1			@ Divido por dos
			mul R4, R4, R7		@ Aplico la máscara

			add R4, R4, R5		@ Agrego los dos valores preparados en la parte alta y en la baja

			ldr R5, [R1]		@ Leo el valor del vector de salida
			sadd16 R5, R5, R4	@ Le sumo el eco al valor del vector de salida
			str R5, [R1], #4	@ Cargo el valor con eco en el vector de salida

			subs R2, 1
			bne asm_eco_eco_loop

		pop {R4-R7}
		bx lr




@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@	void asm_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn);
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@	r2: longitud del vector de entrada
@ Registros a usar
@	r3: contador i
@	r4: contador j
@	r5: indice auxiliar
@	r6: auxiliar para leer R0
@	r7: suma de datos
@	r8: contador de samples
@	r9: resguardo de r2


.thumb_func
	asm_filtroVentana10:
		push {R3-R9}
		mov R3, 0	@ i = 0
		mov R9, R2

		asm_filtroVentana10_bucle:
		mov R4, 5	@ j = 5 - Mitad de la ventana de 10
		mov R5, 0
		mov R6, 0
		mov R7, 0	@ acumulado de suma de datos para promedio
		mov R8, 0	@ contador de samples de la ventana

			asm_filtroVentana10_bucle_izq:	@ Bucle para la parte izquierda de la ventana del filtro
				subs R5, R3, R4			@ Analizo si no me voy de rango
				lsl R5, 1				@ Multiplico por 2 para moverme de a 2 bytes
				ITTT PL					@ Si es positivo o 0
					ldrhPL R6, [R0, R5]	@ Leo el índice correspondiente de R0
					addPL R7, R6		@ Acumulo el valor
					addPL R8, 1			@ Sumo un sample
				subs R4, 1
			bne asm_filtroVentana10_bucle_izq

		mov R4, 5	@ j = 5 - Mitad de la ventana de 10

			asm_filtroVentana10_bucle_der:	@ Bucle para la parte izquierda de la ventana del filtro
				add R5, R3, R4			@ Analizo si no me voy de rango
				cmp R9, R5				@ Comparo con el límite superior del vector
				lsl R5, 1				@ Multiplico por 2 para moverme de a 2 bytes
				ITTT HI					@ Si es positivo o 0
					ldrhHI R6, [R0, R5]	@ Leo el índice correspondiente de R0
					addHI R7, R6		@ Acumulo el valor
					addHI R8, 1			@ Sumo un sample
				subs R4, 1
			bne asm_filtroVentana10_bucle_der

		udiv R7, R7, R8
		strh R7, [R1, R3]

		add R3, 1	@ i++
		subs R2, 1	@ Usado para recorrer el vector
		bne asm_filtroVentana10_bucle

		pop {R3-R9}
		bx lr








