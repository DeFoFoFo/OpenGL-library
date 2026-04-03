#version 430 core

struct Boid
{
    vec3 position;
    vec3 velocity;
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

    float amplitude = 0.1f;
    float speed = 5.0f;
    pos.y += amplitude * pow(abs(pos.x), 2) * cos(time * speed); // Beating wings

    float a = 0.9f;
    float b = 0.2f;
    pos.y -= abs(cos(pos.z * a) * cos(time * speed / 2)) * b;

    gl_Position = projection * view * boid.model * vec4(pos, 1.0f);

    UV = vTexCoords;
}
