#ifndef VEC3_H
#define VEC3_H

typedef struct vec3 {
    union {
        float data[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
} vec3;

vec3 vec3_add(vec3 a, vec3 b);

vec3 vec3_sub(vec3 a, vec3 b);

vec3 vec3_scale(vec3 v, float s);

vec3 vec3_neg(vec3 a);

float vec3_dot(vec3 a, vec3 b);

float vec3_length(vec3 a);

vec3 vec3_normalize(vec3 a);

float vec3_max_element(vec3 a);

#endif