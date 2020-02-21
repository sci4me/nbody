#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "arena.h"
#include "types.h"

void arena_add_page(arena *a) {
    if(a->num_pages == 0) {
        a->pages = calloc(1, sizeof(page));
        a->num_pages++;
    } else {
        a->num_pages++;
        a->pages = realloc(a->pages, sizeof(page) * a->num_pages);
        a->page++;
    }       

    page p;
    p.memory = malloc(a->page_size);
    p.used = 0;
    a->pages[a->page] = p;
}

void arena_init(arena *a, u64 size) {
    assert(size >= 0);

    u64 real_size = 1;
    while(real_size < size) {
        real_size <<= 1;
    }

    a->page_size = real_size;
    a->num_pages = 0;
    a->page = 0;
    a->pages = 0;
}

void arena_free(arena *a) {
    for(u32 i = 0; i < a->num_pages; i++) {
        free(a->pages[i].memory);
    }
    free(a->pages);
}

void* arena_alloc(arena *a, u64 size) {
    if(size == 0) {
        return 0;
    }

    if(a->num_pages == 0) {
        arena_add_page(a);
    }

    page p = a->pages[a->page];

    u64 free = a->page_size - p.used;
    if(size > free) {
        if(a->page < a->num_pages - 1) {
            a->page++;
        } else {
            arena_add_page(a);
        }
        p = a->pages[a->page];
        assert(p.used == 0);
    }

    void *ptr = p.memory + p.used;
    p.used += size;
    a->pages[a->page] = p;
    return ptr;
}

void arena_reset(arena *a) {
    for(u32 i = 0; i < a->num_pages; i++) {
        a->pages[i].used = 0;
    }
    a->page = 0;
}