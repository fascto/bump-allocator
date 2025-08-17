#define _DEFAULT_SOURCE
#include <unistd.h>
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

typedef struct memory_block {
    uint64_t size;               // tamaño de la zona de datos
    uint16_t is_free;            // flag de libre/ocupado
    struct memory_block *next;   // siguiente bloque (para encadenar)
    char *data;                  // comienzo de la memoria de usuario
} memory_block_t;

static memory_block_t *first_block = NULL;
static memory_block_t *last_block = NULL;

// size_t varia segun su tamaño segun la arquitectura que se use: Para 32bits representa 4bytes. Mientras que en 64 vale 8bytes. 
// ASi que lo conviene usarlo para determinar el tamaño de palabra y asi alinear los bits a un multiplo de ese tamaño de palabra.
const size_t MACHINE_WORD = sizeof(size_t);

size_t align(size_t num_of_bytes) {
   
   return (num_of_bytes + MACHINE_WORD - 1) & ~(MACHINE_WORD - 1);
}

// funcion para pedir memoria
void *alloc(size_t bytes_to_alloc) {

    uint64_t bytes_aligned = align(bytes_to_alloc);
    memory_block_t *current = first_block;
    
    while (current)
    {
        if (current->is_free && current->size >= bytes_aligned)
        {
            current->is_free = 0;
            return current->data;
        }
        current = current->next;        
    }

    // Si no encuentra ningun bloque libre que tenga el suficiente tamaño para almacenar la cantidad de bytes entonces
    // se tiene que agregar uno mas.

    memory_block_t *memory_block = sbrk(sizeof(memory_block_t) + bytes_aligned);

    if (memory_block == (void*) -1) 
    {
        return NULL;
    }

    // estructura y metadata al bloque de memoria
    memory_block->is_free = 0;
    memory_block->next = NULL;
    memory_block->size = bytes_aligned;

    if (!first_block)
    {
        first_block = memory_block;
    } else {
        last_block->next = memory_block;
    }
    last_block = memory_block;

    return memory_block->data;

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