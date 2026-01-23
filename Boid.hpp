#pragma once

#include "glm/glm.hpp"

struct Boid
{
    glm::vec3 pos;
    float speed;
    glm::vec3 next_pos;
};