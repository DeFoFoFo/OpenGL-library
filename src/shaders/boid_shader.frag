#version 430 core
in vec3 velocity;

out vec4 fragColor;

void main()
{
    fragColor = vec4(abs(velocity) / 5.0 * 0.5 + 0.5, 1.0);
}