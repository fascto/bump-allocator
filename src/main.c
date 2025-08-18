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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// size_t varia segun su tamaño segun la arquitectura que se use: Para 32bits representa 4bytes. Mientras que en 64 vale 8bytes.
// ASi que lo conviene usarlo para determinar el tamaño de palabra y asi alinear los bits a un multiplo de ese tamaño de palabra.
const size_t MACHINE_WORD = sizeof(size_t);

typedef struct memory_block
{
    uint64_t size;             // tamaño de la zona de datos - 8 bytes
    uint16_t is_free;          // flag de libre/ocupado - 2 bytes
    struct memory_block *next; // siguiente bloque (para encadenar) - 8 bytes / 4 bytes (depende de la arquictura)
} memory_block_t;


typedef struct allocator allocator_t;


struct allocator
{
    void *(*alloc)(allocator_t *, size_t);
    int (*dealloc)(allocator_t *, void *);
    void *state;
} ;

typedef struct heap_data
{
    memory_block_t *first;
    memory_block_t *last;
} heap_data_t;

// BUMP ALLOCATOR IMPLEMENTATION

size_t align(size_t num_of_bytes)
{

    return (num_of_bytes + MACHINE_WORD - 1) & ~(MACHINE_WORD - 1);
}

void *bump_alloc(allocator_t *a, size_t bytes_to_alloc)
{
    heap_data_t *data = (heap_data_t *)a->state;

    uint64_t bytes_aligned = align(bytes_to_alloc);

    memory_block_t *current = data->first;

    while (current)
    {
        if (current->is_free && current->size >= bytes_aligned)
        {
            current->is_free = 0;
            return (void *)(current + 1);
        }
        current = current->next;
    }

    memory_block_t *block = sbrk(sizeof(memory_block_t) + bytes_aligned);
    if (block == (void *)-1)
        return NULL;

    block->size = bytes_aligned;

    block->is_free = 0;
    block->next = NULL;

    if (!data->first)
    {
        data->first = block;
    }
    else
    {
        data->last->next = block;
    }

    data->last = block;
    return (void *)(block + 1);
}

int bump_dealloc(allocator_t *a, void *ptr)
{
    if (!ptr) {
        return -1;
    }

    heap_data_t *data = (heap_data_t *)a->state;

    memory_block_t *block = (memory_block_t *)ptr - 1;

    if (block == data->last)
    {
        sbrk(-(block->size + sizeof(memory_block_t)));
        if (block == data->first)
        {
            data->first = data->last = NULL;
        }
        else
        {
            memory_block_t *curr = data->first;
            while (curr->next != data->last)
            {
                curr = curr->next;
            }
            curr->next = NULL;
            data->last = curr;
        }
        return 0;
    }

    block->is_free = 1;
    return 0;
}

// Funcion para inicilizar el bump_allocator. 
allocator_t bump_allocator()
{
    allocator_t a;
    heap_data_t *data = sbrk(sizeof(heap_data_t));
    
    if (data == (void*)-1) {
        perror("sbrk failed");
        exit(EXIT_FAILURE);
    }  

    data->first = data->last = NULL;
    a.alloc = bump_alloc;
    a.dealloc = bump_dealloc;
    a.state = data;
    return a;
}

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
