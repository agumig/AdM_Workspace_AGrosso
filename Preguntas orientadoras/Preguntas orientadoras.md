#### 1. Describa brevemente los diferentes perfiles de familias de microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias características.
Los microprocesadores ARM se agrupan en tres familias según su utilización:

Cortex A
- Utilizados comunmente para ejecutar y administrar aplicaciones. Son dispositivos en los cuales la latencia no es un parámetro crítico pero sí la performance.

Cortex R
- Utilizados para la ejecución de sistemas operativos en tiempo real. En estas aplicaciones, la latencia y el determinismo son críticos.

Cortex M
- Dispositivos utilizados para desarrollos de fabricación masiva. Contienen características como memoria y periféricos de comunicación embebidos.

		Sección Cortex M

#### 1. Describa brevemente las diferencias entre las familias de procesadores Cortex M0, M3 y M4.
Una de las principales diferencias entre los Cortex M0 y los demás es que los M0 disponen de una arquitectura Von Neumann y los M3 y M4 cuentan con arquitectura Harvard. Las ventajas de la arquitectura Von Neumann es la simpleza y la eficiencia en la utilización de memoria, pero no pueden paralelizar el acceso a datos e instrucciones como sí se logra con la Harvard.

Las otras diferencias se encuentran en las arquitecturas (que definen el set de instrucciones):
M0: set de instrucciones básico.
M3: se incorpora instrucciones para utilizar campos de bit.
M4: se incorpora instrucciones para el procesamiento de grandes cantidades de datos (SIMD) y punto flotante.

#### 2. ¿Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código? Explique
Porque permite que, en la instancia del ensamblado del código, el ensamblador defina si es posible  por ejemplo, en un procesador de 32 bit, utilizar instrucciones para 16 bits en lugar de instrucciones de 32 bits para una determinada operación. Esto vuelve más eficiente el uso de memoria ya que se evita trabajar con cantidades de bits mayores e innecesarios.

####  3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este tipo de arquitectura?
Una arquitectura load/store es aquella que, ante una instrucción, toma los datos necesario de memoria, colocándolos en registros (load) y, luego, toma el resultado de la operación desde los registros correspondientes y los carga en memoria (store). Es decir, las instrucciones usan solamente registros como operandos.

#### 4. ¿Cómo es el mapa de memoria de la familia?
Es un mapa de memoria plano que permite direccionar hasta 4GB.

#### 5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?
El uso de shadowed pointers brinda confiabilidad al sistema evistando que si una aplicación sufre un error grave en la pila, las demás aplicaciones y el kernel no se vean afectados.
Cada tarea tendrá su espacio en la pila y no podrá acceder a sitios del stack fuera del asignado.
#### 6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.
- Modo thread de operación
        - Modo privilegiado de acceso.
        - Modo no privilegiado de acceso.
                Para volver al modo privilegiado, hay que generar una interrupción por software para ir a modo Handler y, desde allí, cambiar el nivel de privilegio.
- Modo handler de operación
        - Es siempre privilegiado
        - Las interrupciones (excepción) se ejecutan en modo handler.
        - Se tiene acceso a todos los recursos del sistema.
		
#### 7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo
En los microcontroladores tradicionales todas las operaciones se realizan sobre el acumulador. Este es una de las entradas de la ALU y, a su vez, la salida.
En un modelo de registros ortogonal, la salida de la ALU va a un registro W y a la memoria de datos, puede guardarse en cualquiera de los dos.
En las instrucciones de doble operando, uno de los datos siempre debe estar en el registro W.
Como ventaja se encuenta que permite un ahorro de instrucciones ya que el resultado de cualquier instrucción que opere con la memoria, puede dejarse en la misma posciión de memoria o en el registro W.

#### 8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo
Las instrucciones de ejecución condicional permiten evitar que haya saltos incondicionales y, por lo tanto, que el pipeline se destruya provocando demoras.


#### 9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).
Las tres primeras excepciones descriptas en la tabla de Excepciones son reset, NMI y HardFault. Estas excepciones ya tienen definido el campo priority y son los niveles más altos de prioridades:
- Reset:	Priority = -3
- NMI:		Priority = -2
- HardFault	Priority = -1

Se describen, a continuación, estas tres principales excepciones:
- Reset:	Excepción por reset del micro.
- NMI:		Excepción no enmascarable. No puede no dispararse el controlador de esta excepción ya que sucede ante situaciones críticas como puede ser una caída de la tensión de alimentación del micro.
- HardFault:	Excepción utilizada para fallos en manejo de memoria, fallos en el bus de datos...

#### 10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?
La pila es un área de memoria que se utiliza para el resguardo de datos, por ejemplo, el resguardo del valor de registros previo a utilizarlos para otra operación.
ARM implementa el uso de los primeros 4 (cuatro) registros de propósito general para los datos pasados a la función, evitando el uso del stack para estas operaciones (mejora de perfomance).
Para el retorno de un valor se utiliza, por definición, el registo r0.
Entonces, una vez que la función finaliza, se debe almacenar el resultado que se debe devolver en el registro r0 y realizar un salto a la dirección guardada en lr (link register) para volver a la instrucción siguiente al llamado de función.

#### 11. Describa la secuencia de reset del microprocesador.
Al realizarse el reset, el microprocesador:
- Va a las dirección 0x0000 (MSP) y carga el Stack Pointer 
- Va a la dirección 0x0004 donde está el handler del reset y lo ejecuta.

#### 12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de los periféricos?
Por core peripherals se entiende a aquellos registros y periféricos propios del procesador y proporcionados por el fabricante. Aquellos periféricos que no son incluídos en el procesador deben incorporarse según especificaciones de su fabricante e implementarse, en algunos casos, mediante su incorporación en el vector de excepciones (si incluyen interrupciones).

#### 13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo
La prioridades de las interrupciones en los Cortex M3 y M4 se encuentran definidas por valores de un ancho de 3 bits como mínimo y 8 como máximo.
Existen tres excepciones con los niveles de prioridades más altos y predefinidos:
- Reset (-3)
- NMI (-2)
- Hard Fault (-1)
Si un periférico será usado a través de interrupción, deberá programarse el handler correspondiente sobre Cortex (NVIC) y configurar el nivel de prioridad.

#### 14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?
El CMSIS es un conjunto de funciones en C que se disponen para utilizar las funcionalidades de la arquitectura cortex.
Permiten lograr un grado de abstracción sobre la arquitectura. Por ejemplo, existen funciones para realizar el cambio de modo de privilegio, esto permite abstraerse del proceso que se necesita para realizar esto (disparar interrupción por software, cambiar LSB de la palabra de CONTROL, etc).

#### 15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo
Al momento de generarse una interrupción, el microprocesador procede a almacenar el contexto de la tarea actual (staking), busca el handler en el vector de excepciones en la entrada del vector correspondiente a la interrupción que se generó y se ejecuta el controlador en modo privilegiado.

#### 16. Explique las características avanzadas de atención a interrupciones: tail chaining y late arrival.
Usado para cuando suceden interrupciones anidadas. 
##### Tail chainning
Al producirse una segunda interrupción de la misma prioridad, en modo handler, se finaliza la ejecución de la ISR1 y se procede a ejecutar la ISR2, sin volver a hacer un staking por la IRS2.

##### Late Arrival
Cuando una ISR de baja prioridad comienza el modo handler pero ocurre una ISR2 de mayor prioridad antes del vector fetch, entonces, se ejecuta el handler de la ISR2.
Si ya se fue al vector fetch a buscar el handler, se anidarán realizando un nuevo stacking.

#### 17. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la portabilidad de los sistemas operativos embebidos?
Es un sistema de interrupciones por timer que permite llevar una marca exacta del tiempo. Al ocurrir una interrupción del systick, se ejuta el scheduler del SO en modo handler.
Favorece la portabilidad ya que ARM exige a los fabricantes que sea un periférico estándar y, por lo tanto, debe incluirse en la fabricación.

#### 18. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?
Es la encargada de proteger regiones definidad de memoria y, por lo tanto, proteger también los accesos a periféricos (mapeados en memoria).

#### 19. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las regiones definidas?
Pueden configurarse, para Cortex-M3 y M4, 8 regiones como máximo. En el caso que una o varias tareas no requieran el uso de toda una región sino de una parte, las regiones de dichas tareas pueden solaparse utilizando el SRD (Sub-Region Disable) y reduciendo, entonces, el número de regiones utilizadas.
Si una región no habilitada por las definiciones de las zonas se solapa con una región habilitada, se aplicarán las reglas de acceso de la segunda. Si una región no habilitada y no solapada es accedida, generará una excepción de MemManage fault.

#### 20. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto de las excepciones? Dé un ejemplo
PendSV es utilizada para el cambio de contexto en Sistemas Operativos embebidos. La excepción PendSV puede ser disparada desde la excepción periódica del SysTick.
En el handler PendSV se deberá realizar varias acciones para el cambio de contexto:
- Resguardar los valores actuales de los registros usados en la tarea actual.
- Resguardar el valor del PSP actual.
- Cargar en el PSP, el SP de la siguiente tarea.
- Restablecer los últimos valores de la siguiente tarea.
- Ir a la siguiente tarea mediante el exception return.

#### 21. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un sistema operativo embebido.
La excepción SVC suele ser utilizada para ejecutar instrucciones desde modo handler. Si se necesita pasar de modo no privilegiado a modo privilegiado, por ejemplo, es necesario escribir el LBS de la palabra de CONTROL; esto puede realizarse, solamente, en modo privilegiado.
Generando el disparo de SVC y reescribiendo el LBS de Control desde el SVC_Handler, este cambio de nive de privilegio es posible.

		Sección ISA
#### 1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo

Los sufijos son variaciones que se le pueden dar a instrucciones para que operen con alguna particularidad adicional. Por ejemplo: 
- sufijos para leer cantidades definidas de bits (ldr una palabra, ldrh media palabra, ldrb un byte).
- para ejecuciones condicionales se utilizan sufijos para indicar la condición bajo la cual debe ejecutarse o no la instrucción.
#### 2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo

El sufijo 's' es utilizado para indicar que al ejecutarse la instrucción deben actualizarse los flags de estado (N, Z, etc).
#### 3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un ejemplo con operaciones con datos de 8 bits.

Las intrucciones de aritmética saturada permiten definir límites para acotar los resultados de la operación que se realiza.
Un ejemplo sería, para 8 bits, utilizar la función USAR R1, #8, R1. Esa función estará acotando el resultado entre 0 y 0xFF en valor de R1.
#### 4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la pila antes de ser modificados?

Se define en la interfaz entre assembler y C el modo de llamado a funciones y el paso de argumentos a las mismas. Los argumentos se reciben en el orden que se pasan y se reciben a través de los registros R0, R1, R2 y R3.
El resultado se retorna a través del registro R0.
Deben resguardarse en la pila todos los valores de los registros de propósito general antes de usarse y, una vez usados, debe restablecerse el valor que fue resguardado.
#### 5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.

Es una instrucción que está preparada para operar con gran cantidad de datos en un ciclo de reloj. Se aplica en operaciones que, justamente, requiera operar con mucha cantidad de dato al mismo tiempo, por ejemplo, procesamiento de imágenes.
