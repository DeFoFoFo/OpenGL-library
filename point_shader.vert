#version 430 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 worldPos = vec3(aPos, 0.0); // scale matters!
    gl_Position = projection * view * model * vec4(worldPos, 1.0);
    gl_PointSize = 1.0;
}