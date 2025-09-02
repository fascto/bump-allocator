#include "arena_allocator.h"

int arena_allocator(arena* arena, size_t capacity, size_t) {

    *arena = (arena) { 
        .buffer = malloc(capacity);  
        .offset = 0;
        .capacity = capacity;
    };

    return 0;
    
} 

int main(int argc, char **argv) {

}