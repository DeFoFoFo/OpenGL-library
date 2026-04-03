#version 430
layout(local_size_x = 256) in;

struct Boid
{
    vec3 position;
    vec3 velocity;
    mat4 model;
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
uniform float minSpeed;
uniform float maxForce;
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

    float cohesionWeight = 10.0f;
    float alignmentWeight = 5.0f;
    float separationWeight = 15.0f;
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
        
        cohesionForce += otherBoid.position - pos;
        alignmentForce += otherBoid.velocity;
        separationForce -= dirToOtherBoid * dist * dist;
    }
    if (neighbours != 0)
    {
        cohesionForce /= neighbours;
        alignmentForce /= neighbours;
        separationForce /= neighbours;
    }

    vec3 desiredVelocity = normalize(cohesionForce) * cohesionWeight 
                      + normalize(alignmentForce) * alignmentWeight
                      + normalize(separationForce) * separationWeight
                      + normalize(avoidanceForce) * avoidanceWeight;

    vec3 steering = desiredVelocity - velocity;
    if (length(steering) > maxForce)
    {
        steering = normalize(steering) * maxForce;
    }

    velocity += steering * dT;
    float speed = length(velocity);
    if (speed > maxSpeed)
    {
        velocity = normalize(velocity) * maxSpeed;
    }
    else if (speed < minSpeed)
    {
        velocity = normalize(velocity) * minSpeed;
    }

    pos += velocity * dT;
    // Clamp it inside the box's dimensions
    pos = mod(pos + boxSize/2, boxSize) - boxSize/2;

    // Compute rotation
    vec3 forward = vec3(0.0f, 0.0f, 1.0f);
    vec3 right = vec3(1.0f, 0.0f, 0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);

    if (length(velocity) > 1e-5)
    {
        forward = normalize(velocity);
        vec3 worldUp = abs(forward.y) < 0.999f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);
        right = normalize(cross(worldUp, forward));
        up = normalize(cross(forward, right));
    }

    mat4 model = mat4(1.0f);
    model[0].xyz = right;
    model[1].xyz = up;
    model[2].xyz = forward;
    model[3].xyz = pos;

    outBoids[idx].position = pos;
    outBoids[idx].velocity = velocity;
    outBoids[idx].model = model;
}