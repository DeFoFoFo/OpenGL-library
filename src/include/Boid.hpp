#pragma once

#include "glm/glm.hpp"

struct Boid
{
    // radius of neighbours is the w component and position the xyz components
    glm::vec4 pos_radius;
    glm::vec4 velocity_neighbourCount;

    Boid(glm::vec4 pos_radius, glm::vec4 velocity_neighbourCount)
        : pos_radius{pos_radius}, velocity_neighbourCount{velocity_neighbourCount}
    {}
};