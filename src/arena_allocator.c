#include "arena_allocator.h"

    void arena_allocator(arena_t* arena, size_t capacity) {
    *arena = (arena_t) { 
        .buffer = malloc(capacity), 
        .offset = 0,
        .capacity = capacity,
    };
} 

void *arena_alloc(arena_t* arena, size_t bytes_to_alloc){
    
    size_t bytes_align = align(bytes_to_alloc);

    if (!(arena->offset+bytes_align < arena->capacity))
    {
        return NULL;
    }
    
    char *data = &arena->buffer[arena->offset]; 
    arena->offset += bytes_align;

    return (void*)data; 
}

void arena_dealloc(arena_t* arena) {
    arena->capacity = 0;
    arena->offset = 0;
    free(arena->buffer);
}

void arena_reset(arena_t* arena) {
    arena->offset = 0;
}


int main(int argc, char **argv) {
    
    arena_t a = {};
    // Creo el la arena y le asigno 16kb
    arena_allocator(&a, (size_t)(16*1024));
    
    char *string = (char*) arena_alloc(&a, 8*1024);

    if (string == NULL) {
        printf("No se pudo asignar memoria\n");
        return 1;
    }
    strcpy(string, "Hola Mundo\n");

    printf("Usando la memoria: %s", string);

    arena_reset(&a);

    arena_dealloc(&a);

    return 0;


}