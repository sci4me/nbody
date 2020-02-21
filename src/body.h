#ifndef BODY_H
#define BODY_H

#include "vec3.h"

typedef struct body {
    float mass;

    vec3 position;
    vec3 velocity;
    vec3 acceleration;

    float red;
    float green;
    float blue;
} body;

void body_update(body *b);

#endif