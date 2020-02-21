#version 440 core

layout (location = 0) uniform mat4 p;
layout (location = 1) uniform mat4 mv;

layout (location = 0) in vec3 pos;

void main() {
    gl_Position = p * mv * vec4(pos, 1.0);
}