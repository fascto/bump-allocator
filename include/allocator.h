#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <stdint.h>

#define MACHINE_WORD sizeof(size_t)

typedef struct allocator allocator_t;

struct allocator {
    void *(*alloc)(allocator_t *, size_t);
    int (*dealloc)(allocator_t *, void *);
    void *state;
};

typedef struct memory_block {
    uint64_t size;
    uint16_t is_free;
    struct memory_block *next;
} memory_block_t;

typedef struct heap_data {
    memory_block_t *first;
    memory_block_t *last;
} heap_data_t;

size_t align(size_t num_of_bytes);

#endif
