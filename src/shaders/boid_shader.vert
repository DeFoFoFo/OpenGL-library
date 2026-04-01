#version 430 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vSpeed;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 speed;

void main()
{
    vec3 fragPos = vec3(model * vec4(vPos.xyz, 1.0));
    gl_PointSize = 2.0f;

    gl_Position = projection * view * vec4(fragPos, 1.0);

    speed = vSpeed.xyz;
}
