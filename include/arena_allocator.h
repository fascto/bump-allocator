#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#define _DEFAULT_SOURCE
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "allocator.h"

typedef struct 
{
    char* buffer;
    size_t capacity;
    size_t offset;
} arena_t;


void *arena_alloc(arena_t* arena, size_t bytes_to_alloc);

void arena_dealloc(arena_t* arena);

void arena_reset(arena_t* arena);

void arena_allocator(arena_t* arena, size_t capacity);

#endif