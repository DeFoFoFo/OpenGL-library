#version 430
layout(local_size_x = 256) in;

struct Boid
{
    vec3 position;
    vec3 velocity;
};

layout(std430, binding = 0) buffer inBoidBuffer
{
    Boid inBoids[];
};

layout(std430, binding = 1) buffer outBoidBuffer
{
    Boid outBoids[];
};

uniform float dT;

uniform float radiusOfInfluence;
uniform float maxSpeed;
uniform float boxSize;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    // The local group exceeds the work group
    if (idx >= inBoids.length()) return;

    Boid boid = inBoids[idx];
    vec3 pos = boid.position;
    vec3 velocity = boid.velocity;

    float neighbours = 0;

    vec3 cohesionForce = vec3(0.0f);
    vec3 alignmentForce = vec3(0.0f);
    vec3 separationForce = vec3(0.0f);
    vec3 avoidanceForce = vec3(0.0f);

    float cohesionWeight = 1.0f;
    float alignmentWeight = 5.0f;
    float separationWeight = 5.0f;
    float avoidanceWeight = 1.0f;

    for (uint i = 0; i < inBoids.length(); ++i)
    {
        // Skip boid
        if (i == idx)
            continue;

        // Skip if the distance between the two boids is greater than the view radius
        Boid otherBoid = inBoids[i];
        vec3 dirToOtherBoid = otherBoid.position - pos;
        float dist = length(dirToOtherBoid);
        if (dist > radiusOfInfluence)
            continue;
        
        // If we are here, the other boid is a neighbour
        neighbours++;
        
        cohesionForce += otherBoid.position;
        alignmentForce += otherBoid.velocity;
        separationForce -= dirToOtherBoid * dist;
    }
    if (neighbours != 0)
    {
        cohesionForce /= neighbours;
        alignmentForce /= neighbours;
        separationForce /= neighbours;
    }
    cohesionForce -= pos;

    vec3 acceleration = (
        cohesionForce * cohesionWeight 
        + alignmentForce * alignmentWeight
        + separationForce * separationWeight
        + avoidanceForce * avoidanceWeight
    );

    velocity += acceleration * dT;
    float speed = length(velocity);
    if (speed > maxSpeed)
    {
        velocity = normalize(velocity) * maxSpeed;
    }

    pos += velocity * dT;
    // Clamp it inside the box's dimensions
    pos = mod(pos + boxSize/2, boxSize) - boxSize/2;

    outBoids[idx].position = pos;
    outBoids[idx].velocity = velocity;
}