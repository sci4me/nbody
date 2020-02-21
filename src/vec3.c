#include <math.h>

#include "vec3.h"

vec3 vec3_add(vec3 a, vec3 b) {
    vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

vec3 vec3_sub(vec3 a, vec3 b) {
    vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

vec3 vec3_scale(vec3 v, float s) {
    vec3 r;
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    return r;
}

vec3 vec3_neg(vec3 a) {
    vec3 r;
    r.x = -a.x;
    r.y = -a.y;
    r.z = -a.z;
    return r;
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(vec3 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

vec3 vec3_normalize(vec3 a) {
    vec3 r;

    float len = a.x * a.x + a.y * a.y + a.z * a.z;
    if(len == 0) {
        r.x = 0.0;
        r.y = 0.0;
        r.z = 0.0;
    } else {
        len = sqrtf(len);
        r.x = a.x / len;
        r.y = a.y / len;
        r.z = a.z / len;
    }
    
    return r;
}

float vec3_max_element(vec3 a) {
    if(a.x > a.y) {
        if(a.y > a.z) {
            return a.y;
        } else {
            return a.z;
        }
    } else {
        return a.x;
    }   
}