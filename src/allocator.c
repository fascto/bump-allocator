#include "allocator.h"

size_t align(size_t num_of_bytes)
{
    return (num_of_bytes + MACHINE_WORD - 1) & ~(MACHINE_WORD - 1);
}