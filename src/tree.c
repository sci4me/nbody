#include "tree.h"
#include "body.h"
#include "vec3.h"
#include "types.h"
#include "arena.h"

/*

'-' means less than 'origin' in that dimension, '+' means greater than or equal to.

child:	0 1 2 3 4 5 6 7
x:      - - - - + + + +
y:      - - + + - - + +
z:      - + - + - + - +

*/

u8 get_octant(vec3 pos, vec3 origin) {
    u8 oct = 0;
    if(pos.x >= origin.x) oct |= 4;
    if(pos.y >= origin.y) oct |= 2;
    if(pos.z >= origin.z) oct |= 1;
    return oct;
}

tree* get_or_create_octant(tree *p, u8 oct) {
    if(p->children[oct] == 0) {
        tree *l = (tree*) arena_alloc(p->arena, sizeof(tree));

        l->arena = p->arena;
        l->b = 0;

        for(u8 i = 0; i < 8; i++) {
            l->children[i] = 0;
        }

        l->half = vec3_scale(p->half, 0.5);

        l->origin.x = p->half.x * (oct & 4 ? 0.5 : -0.5);
        l->origin.y = p->half.y * (oct & 2 ? 0.5 : -0.5);
        l->origin.z = p->half.z * (oct & 1 ? 0.5 : -0.5);

        l->min = vec3_sub(l->origin, l->half);
        l->max = vec3_add(l->origin, l->half);

        l->is_leaf = 1;

        p->children[oct] = l; 
    }
    return p->children[oct];
}

void tree_update_dimensions(tree *t) {
    t->origin = vec3_add(t->min, vec3_scale(vec3_sub(t->max, t->min), 0.5));
    t->half = vec3_scale(vec3_sub(t->max, t->min), 0.5);
}

void tree_insert_body(tree *t, body *b) {
    if(t->is_leaf) {
        t->is_leaf = 0;

        if(t->b == 0) {
            t->b = b;     
        } else {
            body *o = t->b;
            t->b = 0;

            u8 oa = get_octant(o->position, t->origin);
            u8 ob = get_octant(b->position, t->origin);

            tree *ta = get_or_create_octant(t, oa);
            tree *tb = get_or_create_octant(t, ob);

            tree_insert_body(ta, o);
            tree_insert_body(tb, b);
        }
    } else {
        tree *n = get_or_create_octant(t, get_octant(b->position, t->origin));
        tree_insert_body(n, b);
    }
}

void tree_calculate(tree *t, body *b) {
    
}