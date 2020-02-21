#include "body.h"

void body_update(body *b) {
    b->position = vec3_add(b->position, b->velocity);
    b->velocity = vec3_add(b->velocity, b->acceleration);
    
    b->acceleration = (vec3){0,0,0};
}