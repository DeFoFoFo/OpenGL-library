#version 430 core

struct Boid
{
    vec3 position;
    vec3 velocity;
    vec3 acceleration;
    mat4 model;
};

layout(std430, binding = 0) buffer BoidBuffer
{
    Boid boids[];
};

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCoords;

uniform mat4 view;
uniform mat4 projection;

uniform float time;

out vec2 UV;

void main()
{
    uint idx = gl_InstanceID;
    Boid boid = boids[idx];

    vec3 pos = vPos;

    float baseMovement = 3.0f;
    float speed = length(boid.acceleration);
    float amplitude = 0.2f;
    pos.x += amplitude * cos(time * (baseMovement + speed) + pos.z + idx/20);

    gl_Position = projection * view * boid.model * vec4(pos, 1.0f);

    UV = vTexCoords;
}
