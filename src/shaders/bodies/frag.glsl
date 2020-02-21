#version 440 core

in vec3 color;
in vec2 uv;

layout (binding = 0) uniform sampler2D tex;

out vec4 out_color;

void main() {
    float alpha = texture(tex, uv).r;
    out_color = vec4(color * alpha, 1.0);
}