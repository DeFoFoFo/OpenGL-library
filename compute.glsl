#version 430
layout(local_size_x = 256) in;

#define BOX_SIZE 10

struct Boid
{
    vec3 position;
    float speed;
    vec3 next_position;
};

layout(std430, binding = 0) buffer boidBuffer
{
    Boid boids[];
};

uniform float time;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= boids.length()) return;

    boids[idx].next_position.x = boids[idx].position.x + BOX_SIZE/2 + mod(boids[idx].position.x + boids[idx].speed, BOX_SIZE);
    boids[idx].next_position.x -= BOX_SIZE/2;
}