#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vVelocity;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 velocity;

void main()
{
    gl_PointSize = 4.0f;

    gl_Position = projection * view * model * vec4(vPos, 1.0);

    velocity = vVelocity;
}
