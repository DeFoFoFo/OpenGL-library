#pragma once

#include <array>
#include "glm/glm.hpp"

namespace mylib
{

enum class Color : size_t
{
    WHITE,
    GRAY,
    BLACK,
    RED,
    GREEN,
    BLUE,
    count
};

constexpr std::array<glm::vec4, static_cast<size_t>(Color::count)> colorLUT =
{
    glm::vec4(1.0f),                   // WHITE
    glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), // GRAY
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), // BLACK
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // RED
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // GREEN
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)  // BLUE
};

constexpr glm::vec4 color(mylib::Color c)
{
    return colorLUT[static_cast<size_t>(c)];
}

} // namespace mylib