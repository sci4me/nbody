#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "sim.h"
#include "renderer.h"
#include "mat4.h"
#include "util.h"

void program_attach_shader(GLuint program, char *name, GLenum type) {
    const char *source = read_file(name);

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLint info_log_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar info_log[info_log_length + 1];
        glGetShaderInfoLog(id, info_log_length, 0, info_log);
        
        fprintf(stderr, "Error compiling shader `%s`:\n%s\n", name, info_log);
        exit(-1);
    }

    glAttachShader(program, id);
}

GLuint load_program(char *name, GLenum flags) {
    GLuint p = glCreateProgram();

    char buffer[1024]; 

    #define SHADER_TYPE(t, n, s)\
                if(flags & t) {\
                    sprintf(buffer, "src/shaders/%s/%s.glsl", name, n);\
                    program_attach_shader(p, buffer, s);\
                }

    SHADER_TYPE(VERTEX_SHADER, "vert", GL_VERTEX_SHADER)
    SHADER_TYPE(GEOMETRY_SHADER, "geom", GL_GEOMETRY_SHADER)
    SHADER_TYPE(FRAGMENT_SHADER, "frag", GL_FRAGMENT_SHADER)

    #undef SHADER_TYPE

    GLint success;
    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &success);
    if(!success) {
        GLint info_log_length;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar info_log[info_log_length + 1];
        glGetProgramInfoLog(p, info_log_length, 0, info_log);

        fprintf(stderr, "%s\n", &info_log);
        exit(-1);
    }

    return p;
}

void renderer_load_programs(renderer *r) {
    r->bodies_program = load_program("bodies", VERTEX_SHADER | GEOMETRY_SHADER | FRAGMENT_SHADER);
    r->cube_program = load_program("cube", VERTEX_SHADER | FRAGMENT_SHADER);
}

void renderer_generate_flare_texture(renderer *r) {
    glGenTextures(1, &r->flare_texture);
    glBindTexture(GL_TEXTURE_2D, r->flare_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, FLARE_TEXTURE_SIZE, FLARE_TEXTURE_SIZE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    float pixels[FLARE_TEXTURE_SIZE * FLARE_TEXTURE_SIZE];

    float sigma2 = FLARE_TEXTURE_SIZE / 2.0;
    for(s32 i = 0; i < FLARE_TEXTURE_SIZE; i++) {
        float i1 = i - FLARE_TEXTURE_SIZE / 2;
        for(s32 j = 0; j < FLARE_TEXTURE_SIZE; j++) {
            float j1 = j - FLARE_TEXTURE_SIZE / 2;
            pixels[i * FLARE_TEXTURE_SIZE + j] = pow(exp(-((i1 * i1) / (2 * sigma2) + (j1 * j1) / (2 * sigma2))), 2.2);
        }
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, FLARE_TEXTURE_SIZE, FLARE_TEXTURE_SIZE, GL_RED, GL_FLOAT, &pixels);

    glProgramUniform2f(r->bodies_program, 2, FLARE_TEXTURE_SIZE / (2 * (float) r->width), FLARE_TEXTURE_SIZE / (2 * (float) r->height));
}

void renderer_init_buffers(renderer *r, sim *s) {
    {
        glGenVertexArrays(1, &r->bodies_vao);
        glBindVertexArray(r->bodies_vao);

        glGenBuffers(1, &r->bodies_positions_ssbo);
        glGenBuffers(1, &r->bodies_colors_vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(1, 1);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, r->bodies_positions_ssbo);
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, s->num_bodies * sizeof(float) * 3, 0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, r->bodies_colors_vbo);
        glBufferStorage(GL_ARRAY_BUFFER, s->num_bodies * sizeof(float) * 3, 0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

        glBindVertexBuffer(0, r->bodies_positions_ssbo, 0, sizeof(float) * 3);
        glBindVertexBuffer(1, r->bodies_colors_vbo, 0, sizeof(float) * 3);
    }

    {   
        glGenVertexArrays(1, &r->cube_vao);
        glBindVertexArray(r->cube_vao);

        glGenBuffers(1, &r->cube_vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, r->cube_vbo);
        glBufferStorage(GL_ARRAY_BUFFER, 2 * 12 * sizeof(float) * 3, 0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

        glBindVertexBuffer(0, r->cube_vbo, 0, sizeof(float) * 3);
    }
}   

void renderer_update_bodies(renderer *r, sim *s) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, r->bodies_positions_ssbo);
    float *m = (float*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    for(s32 i = 0; i < s->num_bodies; i++) {
        s32 j = i * 3;
        m[j] = s->bodies[i].position.x;
        m[j + 1] = s->bodies[i].position.y;
        m[j + 2] = s->bodies[i].position.z;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, r->bodies_colors_vbo);
    m = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(s32 i = 0; i < s->num_bodies; i++) {
        s32 j = i * 3;
        m[j] = s->bodies[i].red;
        m[j + 1] = s->bodies[i].green;
        m[j + 2] = s->bodies[i].blue;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void renderer_update_cube_vbo(renderer *r) {
    float sz = 11;

    glBindBuffer(GL_ARRAY_BUFFER, r->cube_vbo);
    float *m = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    s32 i = 0;
    #define EMIT(x, y, z) m[i++] = x; m[i++] = y; m[i++] = z;

    EMIT(-sz, -sz, -sz)
    EMIT( sz, -sz, -sz)
    EMIT(-sz, -sz,  sz)
    EMIT( sz, -sz,  sz)
    EMIT(-sz,  sz, -sz)
    EMIT( sz,  sz, -sz)
    EMIT(-sz,  sz,  sz)
    EMIT( sz,  sz,  sz)
    EMIT(-sz, -sz, -sz)
    EMIT(-sz,  sz, -sz)
    EMIT(-sz, -sz,  sz)
    EMIT(-sz,  sz,  sz)
    EMIT( sz, -sz, -sz)
    EMIT( sz,  sz, -sz)
    EMIT( sz, -sz,  sz)
    EMIT( sz,  sz,  sz)
    EMIT(-sz, -sz, -sz)
    EMIT(-sz, -sz,  sz)
    EMIT(-sz,  sz, -sz)
    EMIT(-sz,  sz,  sz)
    EMIT( sz, -sz, -sz)
    EMIT( sz, -sz,  sz)
    EMIT( sz,  sz, -sz)
    EMIT( sz,  sz,  sz)

    #undef EMIT

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

#ifdef GL_DEBUG
void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    if(severity == GL_DEBUG_SEVERITY_NOTIFICATION || severity == GL_DEBUG_SEVERITY_LOW) {
        return;
    }
    
    printf("%s\n", message);    
    fflush(stdout);
}
#endif

void renderer_init(renderer *r, sim *s) {
    #ifdef GL_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROCARB) gl_debug_callback, 0);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
    #endif

    renderer_load_programs(r);
    renderer_generate_flare_texture(r);
    renderer_init_buffers(r, s);

    renderer_update_bodies(r, s);
    renderer_update_cube_vbo(r);

    r->p = mat4_infinite_perspective(90 * M_PI / 180, r->width / (float) r->height, 1.0);
    glProgramUniformMatrix4fv(r->bodies_program, 0, 1, GL_FALSE, r->p.data);
    glProgramUniformMatrix4fv(r->cube_program, 0, 1, GL_FALSE, r->p.data);

    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
}

float t = 0;

void renderer_update(renderer *r, sim *s) {
    t += 0.001;
}

void renderer_render(renderer *r, sim *s) {
    if(r->resized) {
        r->resized = 0;

        glViewport(0, 0, r->width, r->height);
        
        glProgramUniform2f(r->bodies_program, 2, FLARE_TEXTURE_SIZE / (2 * (float) r->width), FLARE_TEXTURE_SIZE / (2 * (float) r->height));
        
        r->p = mat4_infinite_perspective(90 * M_PI / 180, r->width / (float) r->height, 1.0);
        glProgramUniformMatrix4fv(r->bodies_program, 0, 1, GL_FALSE, r->p.data);
        glProgramUniformMatrix4fv(r->cube_program, 0, 1, GL_FALSE, r->p.data);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 mv = mat4_mul(mat4_mul(mat4_rotate(t * 100, 0, 0, 1), mat4_rotate(60, 1, 0, 0)), mat4_translate(0, 0, -25));

    {    
        renderer_update_bodies(r, s);
        glBindVertexArray(r->bodies_vao);
        glUseProgram(r->bodies_program);
        glProgramUniform3f(r->bodies_program, 3, s->root->min.x, s->root->min.y, s->root->min.z);
        glProgramUniform3f(r->bodies_program, 4, s->root->max.x, s->root->max.y, s->root->max.z);
        glProgramUniformMatrix4fv(r->bodies_program, 1, 1, GL_FALSE, mv.data);
        glBindTexture(GL_TEXTURE_2D, r->flare_texture);
        glDrawArrays(GL_POINTS, 0, s->num_bodies);
    }

    {
        glBindVertexArray(r->cube_vao);
        glUseProgram(r->cube_program);
        glProgramUniformMatrix4fv(r->cube_program, 1, 1, GL_FALSE, mv.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDrawArrays(GL_LINES, 0, 24);
    }
}

void renderer_shutdown(renderer *r, sim *s) {

}