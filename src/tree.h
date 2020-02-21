#ifndef TREE_H
#define TREE_H

#include "body.h"
#include "types.h"
#include "vec3.h"
#include "arena.h"

typedef struct tree {
    arena *arena;

    float mass;

    vec3 min;
    vec3 max;    
    vec3 origin;
    vec3 half;

    vec3 center_of_mass;

    body *b;

    u8 is_leaf;

    struct tree *children[8];
} tree;

void tree_update_dimensions(tree *t);

void tree_insert_body(tree *t, body *b);

void tree_calculate(tree *t, body *b);

#endif