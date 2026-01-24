#version 430
layout(local_size_x = 256) in;

struct Boid
{
    vec4 position_speed;
};

layout(std430, binding = 0) buffer inBoidBuffer
{
    Boid inBoids[];
};

layout(std430, binding = 1) buffer outBoidBuffer
{
    Boid outBoids[];
};

uniform float time;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= inBoids.length()) return;

    vec3 pos = inBoids[idx].position_speed.xyz;
    float speed = inBoids[idx].position_speed.w;

    float distance_from_origin = length(pos); 

    float angle = time * idx * 0.005;
    pos.x = cos(angle) * distance_from_origin;
    pos.y = sin(angle) * distance_from_origin;
    outBoids[idx].position_speed = vec4(pos, speed);
}