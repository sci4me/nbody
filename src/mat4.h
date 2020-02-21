#ifndef MAT4_H
#define MAT4_H

#include "vec3.h"

/*

| 0   1   2   3  |
| 4   5   6   7  |
| 8   9   10  11 |
| 12  13  14  15 |

*/

typedef struct mat4 {
    float data[16];
} mat4;

void mat4_print(mat4 m);

mat4 mat4_identity();

mat4 mat4_translate(float x, float y, float z);

mat4 mat4_rotate(float angle, float x, float y, float z);

mat4 mat4_infinite_perspective(float fovy, float aspect, float zNear);

mat4 mat4_mul(mat4 a, mat4 b);

vec3 mat4_transform(mat4 m, vec3 v);

#endif