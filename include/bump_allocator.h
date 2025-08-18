#ifndef BUMP_ALLOCATOR_H
#define BUMP_ALLOCATOR_H

#define _DEFAULT_SOURCE
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "allocator.h"

void *bump_alloc(allocator_t *a, size_t bytes_to_alloc);

int bump_dealloc(allocator_t *a, void *ptr);

allocator_t bump_allocator();


#endif