.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_svc
.global asm_sum
.global asm_zero
.global asm_productoEscalar32
.global asm_productoEscalar16


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
		push {r2}			@ Resguardo el valor de r2
		mov r2, 0			@ r2 = 0.
		asm_zeros_bucle:	@ Etiqueta de comienzo de bucle.
			str r2, [r0]	@ Seteo en 0 el contenido de la dirección apuntada por r0.
			add r0, 4		@ Me desplazo 4 bytes en la memoria.
			subs r1, 1		@ Resto en 1 la longitud y actualizo flags del procesador.
			bne asm_zeros_bucle	@ Mientras Z != 0, se salta a la etiqueta de bucle.
		pop {r2}
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
		push {r4}				@ Guardo el registro que voy a usar
		asm_prod32_bucle:		@ Etiqueta de bucle
			ldr r4, [r0], #4	@ r4 = *r0;  r0 = r0 + 4 bytes
			mul r4, r3			@ r4 = r4 * r3
			str r4, [r1], #4	@ *r1 = r4; r1 = r1 + 4 bytes
			subs r2, 1			@ Resto en 1 la longitud y actualizo flags del procesador.
			bne asm_prod32_bucle	@ Mientras Z != 0, se salta a la etiqueta de bucle.
		pop {r4}
		bx lr		@ Retorno a quien llamó a la función.


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);
@
@ Parametros recibidos:
@   r0: dirección de memoria de inicio del vector de entrada.
@   r1: dirección de memoria de inicio del vector de salida.
@	r2: longitud del vector de entrada.
@	r3: valor escalar a usar en la multiplicación.
.thumb_func
	asm_productoEscalar16:
		push {r4}				@ Guardo el registro que voy a usar
		asm_prod16_bucle:		@ Etiqueta de bucle
			ldrh r4, [r0], #2	@ r4 = *r0;  r0 = r0 + 4 bytes
			mul  r4, r3			@ r4 = r4 * r3
			strh r4, [r1], #2	@ *r1 = r4; r1 = r1 + 4 bytes
			subs r2, 1			@ Resto en 1 la longitud y actualizo flags del procesador.
			bne asm_prod16_bucle	@ Mientras Z != 0, se salta a la etiqueta de bucle.
		pop {r4}
		bx lr		@ Retorno a quien llamó a la función.

