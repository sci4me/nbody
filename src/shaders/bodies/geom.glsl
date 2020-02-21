#version 440 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (location = 2) uniform vec2 flare_size;

in vec4 pass_pos[];
in vec3 pass_col[];

out vec3 color;
out vec2 uv;

void main() {
    color = pass_col[0];

    vec4 pos = pass_pos[0];
    vec2 f = flare_size * 35;

    gl_Position = pos + vec4(-f.x, -f.y, 0, 0);
    uv = vec2(0, 0);
    EmitVertex();

    gl_Position = pos + vec4(f.x, -f.y, 0, 0);
    uv = vec2(1, 0);
    EmitVertex();

    gl_Position = pos + vec4(-f.x, f.y, 0, 0);
    uv = vec2(0, 1);
    EmitVertex();

    gl_Position = pos + vec4(f.x, f.y, 0, 0);
    uv = vec2(1, 1);
    EmitVertex();

    EndPrimitive();
}