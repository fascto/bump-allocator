
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          SEGMENTOS DE MEMORIA DE UN PROCESO                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Primero un poco de contexto, el stack es un estructura de datos estatica con un tamaño definido.
// Las direcciones bajas de memoria a la mas alta:

// TEXT: se le dice le "code segment" o "text segment", es donde se guardan las instrucciones de del programa.
// Tiene el codigo ejecutable, las funciones, declaraciones y demas cuestiones para que el programa funque.
// Este pedazo de memoria es de solo lectura y se comparte con demas partes del programa.

// DATA SEGMENT: luego del text segment le sigue el data segment el cual se divide en 2, "initialized data segment" (.data) y
// "uninitilized data" (.bss)

//// el primer segmento almacena las variables globales y estaticas inicializadas (que tiene un valor explicito asignados).
//// Osea que cuando inicializamos una variable, se esta reservando y posteriormente asignando en esta parte de la memoria. (tiempo de compilacion).

//// Luego tenemos el otro segmento que no esta inicializada de manera explicita variables globales y estaticas. Ejemplo:
//// int global; ó int static elpepe;

// STACK: El STACK es un segmento de memoria que esta en las direcciones mas alta de memoria y que va a crecer mientras se hacen llamada de funciones.
// Lo que se crea es un stack frame, el cual tiene las variables locales, la direccion de retorno (para saber donde termina la funcion),
// los parametros de la funcion si los tiene.

// Por ultimo esta el HEAP que es donde vamos a alojar memoria de manera dinamica con algun allocator como lo es malloc. LO que nos indica
// que esta memoria puede ser accedida mediante una referencia usando punteros. Esto es lo que vamos a intentar "replicar".

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// size_t varia segun su tamaño segun la arquitectura que se use: Para 32bits representa 4bytes. Mientras que en 64 vale 8bytes.
// ASi que lo conviene usarlo para determinar el tamaño de palabra y asi alinear los bits a un multiplo de ese tamaño de palabra.


#include "bump_allocator.h"
#include "allocator.h"  


int main(void)
{
    allocator_t my_alloc = bump_allocator();

    printf("HEAP INICIAL: 0x%lx\n", (uintptr_t)my_alloc.alloc(&my_alloc, 0));

    void *p1 = my_alloc.alloc(&my_alloc, 10);
    printf("p1(10bytes): 0x%lx\n", (uintptr_t) p1);
    void *p2 = my_alloc.alloc(&my_alloc, 20);
    printf("p2 (20 bytes): 0x%lx\n", (uintptr_t) p2);

    int result = my_alloc.dealloc(&my_alloc, p2);
    
    if (result == 0)
    {
        printf("p2 ha sido liberado (marcado como libre)\n");
    } else {
        printf("p2 no se ha podido liberar\n");
    }
    
    void *p3 = my_alloc.alloc(&my_alloc, 16);
    printf("p3(16bytes): 0x%lx\n", (uintptr_t) p3);

    my_alloc.dealloc(&my_alloc, p1);
    my_alloc.dealloc(&my_alloc, p3);

    return 0;
}
