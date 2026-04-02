#pragma once

#include "glm/glm.hpp"

struct Boid
{
    glm::vec3 position;
    float _pad0; // glsl vec3 is 16 bytes
    glm::vec3 velocity;
    float _pad1;

    Boid(glm::vec3 pos, glm::vec3 velocity)
        : position{pos}, velocity{velocity}
    {}
};