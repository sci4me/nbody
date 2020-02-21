#version 440 core

layout (location = 0) uniform mat4 p;
layout (location = 1) uniform mat4 mv;

layout (location = 3) uniform vec3 pmin;
layout (location = 4) uniform vec3 pmax;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec4 pass_pos;
out vec3 pass_col;

vec3 map(vec3 value, vec3 inMin, vec3 inMax, vec3 outMin, vec3 outMax) {
    //return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
    return vec3(
    	outMin.x + (outMax.x - outMin.x) * (value.x - inMin.x) / (inMax.x - inMin.x),
    	outMin.y + (outMax.y - outMin.y) * (value.y - inMin.y) / (inMax.y - inMin.y),
    	outMin.z + (outMax.z - outMin.z) * (value.z - inMin.z) / (inMax.z - inMin.z)
    );
}

void main() {
    vec3 spos = map(pos, pmin, pmax, vec3(-10, -10, -10), vec3(10, 10, 10));
    pass_pos = p * mv * vec4(spos, 1.0);
    pass_col = col;
}