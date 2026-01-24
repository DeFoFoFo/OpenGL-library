#pragma once

#include "glm/glm.hpp"

struct Boid
{
    glm::vec4 pos_speed;

    Boid(glm::vec4 pos_speed = glm::vec4(0.0f))
        : pos_speed{pos_speed}
    {}
};