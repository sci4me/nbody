#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "util.h"
#include "arena.h"
#include "sim.h"

void sim_init(sim *s) {
    s->num_bodies = 100;
    s->bodies = malloc(sizeof(body) * s->num_bodies);

    for(int i = 0; i < s->num_bodies; i++) {
        body b;

        b.mass = 100;

        b.position.x = randm11() * 10;
        b.position.y = randm11() * 10;
        b.position.z = randm11() * 10;

        b.velocity.x = 0;
        b.velocity.y = 0;
        b.velocity.z = 0;
        
        b.acceleration.x = 0;
        b.acceleration.y = 0;
        b.acceleration.z = 0;

        b.red = 1;
        b.green = 0;
        b.blue = 1;

        s->bodies[i] = b;
    }

    s->tree_arena = malloc(sizeof(arena));
    arena_init(s->tree_arena, 1024);
}

void sim_update(sim *s) {
    arena_reset(s->tree_arena);

    s->root = (tree*) arena_alloc(s->tree_arena, sizeof(tree));
    s->root->min = (vec3){99999,99999,99999};
    s->root->max = (vec3){-99999,-99999, -99999};
    s->root->arena = s->tree_arena;
    s->root->is_leaf = 1;
    s->root->b = 0;
    for(u8 i = 0; i < 8; i++) {
        s->root->children[i] = 0;
    }

    for(u32 i = 0; i < s->num_bodies; i++) {
        body b = s->bodies[i];

        if(b.position.x < s->root->min.x) { s->root->min.x = b.position.x; }
        if(b.position.y < s->root->min.y) { s->root->min.y = b.position.y; }
        if(b.position.z < s->root->min.z) { s->root->min.z = b.position.z; }
        if(b.position.x > s->root->max.x) { s->root->max.x = b.position.x; }
        if(b.position.y > s->root->max.y) { s->root->max.y = b.position.y; }
        if(b.position.z > s->root->max.z) { s->root->max.z = b.position.z; }
    }

    tree_update_dimensions(s->root);

    for(u32 i = 0; i < s->num_bodies; i++) {
        tree_insert_body(s->root, &s->bodies[i]);
    }

    for(u32 i = 0; i < s->num_bodies; i++) {
        body_update(&s->bodies[i]);
    }
}

void sim_shutdown(sim *s) {

}