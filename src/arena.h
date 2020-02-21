#ifndef ARENA_H
#define ARENA_H

#include "types.h"

typedef struct page {
    void *memory;
    u64 used;
} page;     

typedef struct arena {
    u64 page_size;
    u32 num_pages;
    u32 page;
    page *pages;
} arena;

void arena_init(arena *a, u64 page_size);

void arena_free(arena *a);

void* arena_alloc(arena *a, u64 size);

void arena_reset(arena *a);

#endif