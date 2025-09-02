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

void *arena_alloc(allocator_t *a, size_t bytes_to_alloc);

int arena_dealloc(allocator_t *a, void *ptr);

allocator_t arena_allocator();

typedef struct 
{
    char* buffer;
    size_t capacity;
    size_t offset;
} arena;

#endif