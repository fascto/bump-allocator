#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define TAMAÑO_PALABRA 8

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


typedef struct header header_t;
typedef struct content content_t;

typedef struct memory_block memory_block_t;

struct header {
    uint32_t size; // 4 byte
    uint32_t *next; // 4 byte
    uint16_t is_free; // 4 byte
};

struct content {
    char *data;
};

struct memory_block {
    header_t header;
    content_t content;
};

int align(size_t num_of_bytes) {
   
   return (num_of_bytes + TAMAÑO_PALABRA - 1) & ~(TAMAÑO_PALABRA - 1);
}

// funcion para pedir memoria
void *alloc(size_t bytes_to_alloc) {
    // TODO

    uint32_t bytes_aligned = align(bytes_to_alloc);

    void *current = sbrk(0);

    sbrk(bytes_aligned);

    return current;
}


// funcion para devolver memoria
void dealloc(void *mem) {
    // TODO

    // Revisar primero si el ultimo bloque es el que hay que liberar

    // Si no buscar el bloque y hacer que se marque como "libre"
}



int main(void) {

    // BRK -> Me va a tirar la direccion de donde arranca el heap (o mejor dicho donde se encuentra el program break pointer)

    void *heap = sbrk(0);
    // La direccion de memoria donde me dejo (Es el final del heap LOGICO que el kernel me da). Despues pega un salto a otras direcciones
    // QUe el kernel hace que se extienda el tamaño dle heap.

    printf("HEAP: 0x%lx\n", (uintptr_t)heap);

    void *elpepe = alloc(3);
    printf("Primera reserva: 0x%lx\n", (uintptr_t)elpepe);

    void *colapinto = alloc(16);
    printf("Segunda reserva: 0x%lx\n", (uintptr_t)colapinto);




    return 0;

}