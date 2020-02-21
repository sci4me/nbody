#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "types.h"
#include "mat4.h"
#include "sim.h"
#include "renderer.h"

static renderer *the_r;
static sim *the_s;

void error_callback(int error, const char* description) {
	printf("GLFW error: %s\n", description);
}

void resize_callback(GLFWwindow *window, int width, int height) {
    // is this a data race? maybe?
    the_r->width = width;
    the_r->height = height;
    the_r->resized = 1;
}

int main(int argc, char **argv) {
    if (!glfwInit()) {
        fprintf(stderr, "Couldn't initialize GLFW3\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
	glfwWindowHint(GLFW_SAMPLES, 4); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    #ifdef GL_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  
    #endif

    u32 WIDTH = 1280;
    u32 HEIGHT = 720;

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "B1", 0, 0);
    if(!window) {
        printf("Couldn't create window\n");

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, resize_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Couldn't initialize GLEW\n");
        return -1;
    }

    time_t t;
    srand((unsigned) time(&t));

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSwapInterval(0);

    double last = glfwGetTime();
    double now;
    double timer = last;
    double delta = 0;

    u32 updates = 0;     
    u32 frames = 0;

    sim s;
    sim_init(&s);

    renderer r;
    r.width = WIDTH;
    r.height = HEIGHT;
    renderer_init(&r, &s);

    the_r = &r;
    the_s = &s;

    sim_update(&s); // prevent accidentally trying to reference the root node before it is initialized
    
    while (!glfwWindowShouldClose(window)) {
		now = glfwGetTime();
        delta += (now - last) * 60.0;
        last = now;

        while(delta >= 1.0) {
            sim_update(&s);
            updates++;
            delta--;

            if(delta < 1.0) {
                renderer_update(&r, &s);
            }
        }

        renderer_render(&r, &s);
        frames++;

        if(glfwGetTime() - timer > 1.0) {
            timer++;
            printf("FPS: %u  UPS: %u\n", frames, updates);
            fflush(stdout);
            frames = 0;
            updates = 0;
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    sim_shutdown(&s);

	glfwTerminate();
    return 0;
}