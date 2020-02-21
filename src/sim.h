#ifndef SIM_H
#define SIM_H

#include "types.h"
#include "arena.h"
#include "tree.h"
#include "body.h"

typedef struct sim {
    u32 num_bodies;
    body *bodies;
    arena *tree_arena;
    tree *root;
} sim;

void sim_init(sim *s);

void sim_update(sim *s);

void sim_shutdown(sim *s);

#endif