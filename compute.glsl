#version 430
layout(local_size_x = 256) in;

layout(std430, binding = 0) buffer posBuffer {
    vec2 positions[];
};

uniform float time;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= positions.length()) return;

    positions[idx].x += 0.001*cos(time);
    positions[idx].y += 0.001*sin(time);
}