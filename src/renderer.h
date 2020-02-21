#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>

#include "mat4.h"
#include "sim.h"

#define GL_DEBUG

#define VERTEX_SHADER 1
#define GEOMETRY_SHADER 2
#define FRAGMENT_SHADER 4

#define FLARE_TEXTURE_SIZE 32

typedef struct renderer {
    u32 width;
    u32 height;
    u8 resized;

    GLuint bodies_program;
    GLuint bodies_vao;
    GLuint bodies_positions_ssbo;
    GLuint bodies_colors_vbo;
    
    GLuint cube_program;
    GLuint cube_vao;
    GLuint cube_vbo;

    GLuint flare_texture;
    mat4 p;
} renderer;

void renderer_init(renderer *r, sim *s);

void renderer_update(renderer *r, sim *s);

void renderer_render(renderer *r, sim *s);

void renderer_shutdown(renderer *r, sim *s);

#endif