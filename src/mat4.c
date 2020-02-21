#include <stdio.h>
#include <math.h>

#include "util.h"
#include "mat4.h"
#include "vec3.h"

void mat4_print(mat4 m) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            printf("%- 6.1f ", m.data[i * 4 + j]);
        }
        printf("\n");
    }
}

mat4 mat4_identity() {
    mat4 m;

    m.data[0] = 1;    m.data[4] = 0;    m.data[8] = 0;     m.data[12] = 0;
    m.data[1] = 0;    m.data[5] = 1;    m.data[9] = 0;     m.data[13] = 0;
    m.data[2] = 0;    m.data[6] = 0;    m.data[10] = 1;    m.data[14] = 0;
    m.data[3] = 0;    m.data[7] = 0;    m.data[11] = 0;    m.data[15] = 1;

    return m;
}

mat4 mat4_translate(float x, float y, float z) {
    mat4 m;

    m.data[0] = 1;    m.data[4] = 0;    m.data[8] = 0;     m.data[12] = x;
    m.data[1] = 0;    m.data[5] = 1;    m.data[9] = 0;     m.data[13] = y;
    m.data[2] = 0;    m.data[6] = 0;    m.data[10] = 1;    m.data[14] = z;
    m.data[3] = 0;    m.data[7] = 0;    m.data[11] = 0;    m.data[15] = 1;

    return m;
}

mat4 mat4_rotate(float angle, float x, float y, float z) {
    mat4 m;

    float c = cos(angle * M_PI / 180);
    float s = sin(angle * M_PI / 180);
    float c1 = 1 - c;

    m.data[0] = x * x * c1 + c;         m.data[4] = x * y * c1 + z * s;     m.data[8] = x * z * c1 - y * s;     m.data[12] = 0;
    m.data[1] = x * y * c1 - z * s;     m.data[5] = y * y * c1 + c;         m.data[9] = y * z * c1 + x * s;     m.data[13] = 0;
    m.data[2] = x * z * c1 + y * s;     m.data[6] = y * z * c1 - x * s;     m.data[10] = z * z * c1 + c;        m.data[14] = 0;
    m.data[3] = 0;                      m.data[7] = 0;                      m.data[11] = 0;                     m.data[15] = 1;

    return m;
}

mat4 mat4_infinite_perspective(float fovy, float aspect, float zNear) {
    float range = tan(fovy / 2) * zNear;
    float left = -range * aspect;
    float right = range * aspect;
    float bottom = -range;
    float top = range;

    mat4 r;

    r.data[0] = 2 * zNear / (right - left);    r.data[4] = 0;                            r.data[8] = 0;     r.data[12] = 0;
    r.data[1] = 0;                             r.data[5] = 2 * zNear / (top - bottom);   r.data[9] = 0;     r.data[13] = 0;
    r.data[2] = 0;                             r.data[6] = 0;                            r.data[10] = -1;   r.data[14] = -2 * zNear;
    r.data[3] = 0;                             r.data[7] = 0;                            r.data[11] = -1;   r.data[15] = 0;

    return r;
}

mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 r;

    for(int i = 0; i < 16; i++) {
        r.data[i] = 0;
    }

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                r.data[i * 4 + j] += a.data[i * 4 + k] * b.data[k * 4 + j];
            }
        }    
    }

    return r;
}

vec3 mat4_transform(mat4 m, vec3 v) {
    vec3 r;

    r.x = v.x * m.data[0] + v.y * m.data[4] + v.z * m.data[8] + v.x * m.data[12];
    r.y = v.x * m.data[1] + v.y * m.data[5] + v.z * m.data[9] + v.y * m.data[13]; 
    r.z = v.x * m.data[2] + v.y * m.data[6] + v.z * m.data[10] + v.z * m.data[14];

    return r;
}
