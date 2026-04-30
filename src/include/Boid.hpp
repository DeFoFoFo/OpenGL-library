#pragma once

#include "glm/glm.hpp"

struct Boid
{
    glm::vec3 position;
    float _pad0; // glsl vec3 is 16 bytes
    glm::vec3 velocity;
    float _pad1;
    glm::vec3 acceleration;
    float _pad2;
    glm::mat4 model;

    Boid(glm::vec3 pos, glm::vec3 velocity)
        : position{ pos }, velocity{ velocity }, acceleration{ glm::vec3(0.0f) }, model{ glm::mat4(1.0f) }
    {
    }
};