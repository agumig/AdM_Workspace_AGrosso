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


#### 6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.

- Modo thread de operación
        - Modo privilegiado de acceso.
        - Modo no privilegiado de acceso.
                Para volver al modo privilegiado, hay que generar una interrupción por software para ir a modo Handler y, desde allí, cambiar el nivel de privilegio.
- Modo handler de operación
        - Es siempre privilegiado
        - Las interrupciones (excepción) se ejecutan en modo handler.
        - Se tiene acceso a todos los recursos del sistema.

### 8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo
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

#### 14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?
El CMSIS es un conjunto de funciones en C que se disponen para utilizar las funcionalidades de la arquitectura cortex.
Permiten lograr un grado de abstracción sobre la arquitectura. Por ejemplo, existen funciones para realizar el cambio de modo de privilegio, esto permite abstraerse del proceso que se necesita para realizar esto (disparar interrupción por software, cambiar LSB de la palabra de CONTROL, etc).


#### 17. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la portabilidad de los sistemas operativos embebidos?

Es un sistema de interrupciones por timer que permite llevar una marca exacta del tiempo. Al ocurrir una interrupción del systick, se ejuta el scheduler del SO en modo handler.
Favorece la portabilidad ya que ARM exige a los fabricantes que sea un periférico estándar y, por lo tanto, debe incluirse en la fabricación.

#### 18. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?

Es la encargada de proteger regiones definidad de memoria y, por lo tanto, proteger también los accesos a periféricos (mapeados en memoria).

#### 21. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un sistema operativo embebido.
La excepción SVC suele ser utilizada para ejecutar instrucciones desde modo handler. Si se necesita pasar de modo no privilegiado a modo privilegiado, por ejemplo, es necesario escribir el LBS de la palabra de CONTROL; esto puede realizarse, solamente, en modo privilegiado.
Generando el disparo de SVC y reescribiendo el LBS de Control desde el SVC_Handler, este cambio de nive de privilegio es posible.

		Sección ISA

#### 5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.

Es una instrucción que está preparada para operar con gran cantidad de datos en un ciclo de reloj. Se aplica en operaciones que, justamente, requiera operar con mucha cantidad de dato al mismo tiempo, por ejemplo, procesamiento de imágenes.
