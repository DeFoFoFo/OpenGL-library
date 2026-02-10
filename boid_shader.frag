#version 430 core
in vec3 speed;

out vec4 fragColor;

void main()
{
    fragColor = vec4(speed, 1.0); // More color diversity if you don't take the absolute value
}