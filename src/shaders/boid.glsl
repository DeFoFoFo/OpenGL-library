#version 430
layout(local_size_x = 256) in;

#define BOX_SIZE 100

struct Boid
{
    vec4 position_radius;
    vec4 velocity_neighborCount;
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

void main() {
    uint idx = gl_GlobalInvocationID.x;
    // The local group exceeds the work group
    if (idx >= inBoids.length()) return;

    Boid this_boid = inBoids[idx];
    vec3 pos = this_boid.position_radius.xyz;
    vec3 velocity = this_boid.velocity_neighborCount.xyz;

    float neighbour_count = 0;
    float radius_neighbours = this_boid.position_radius.w;

    vec3 cohesion_force = vec3(0.0f);
    vec3 alignment_force = vec3(0.0f);
    vec3 separation_force = vec3(0.0f);
    vec3 avoidance_force = vec3(0.0f);

    float cohesion_weight = 1.0f;
    float alignment_weight = 5.0f;
    float separation_weight = 2.0f;
    float avoidance_weight = 1.0f;

    for (uint i = 0; i < inBoids.length(); ++i)
    {
        // Skip this_boid
        if (i == idx)
            continue;

        // Skip if the distance between the two boids is greater than the view radius
        Boid other_boid = inBoids[i];
        vec3 direction_to_other_boid = other_boid.position_radius.xyz - pos;
        float dist = length(direction_to_other_boid);
        if (dist > radius_neighbours)
            continue;
        
        // If we are here, the other boid is a neighbour
        neighbour_count++;
        
        cohesion_force += other_boid.position_radius.xyz;
        alignment_force += other_boid.velocity_neighborCount.xyz;
        separation_force -= direction_to_other_boid * dist;
    }
    if (neighbour_count != 0)
    {
        cohesion_force /= neighbour_count;
        alignment_force /= neighbour_count;
        separation_force /= neighbour_count;
    }
    cohesion_force -= pos;

    vec3 acceleration = (
        cohesion_force * cohesion_weight 
        + alignment_force * alignment_weight
        + separation_force * separation_weight
        + avoidance_force * avoidance_weight
    );

    velocity += acceleration * dT;
    velocity = clamp(velocity, vec3(-2.0), vec3(2.0));

    pos += velocity * dT;
    // Clamp it inside the box's dimensions
    pos = mod(pos + BOX_SIZE/2, BOX_SIZE) - BOX_SIZE/2;

    outBoids[idx].position_radius = vec4(pos, radius_neighbours);
    outBoids[idx].velocity_neighborCount = vec4(velocity, neighbour_count);
}