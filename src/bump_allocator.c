#include "bump_allocator.h"


// BUMP ALLOCATOR IMPLEMENTATION

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