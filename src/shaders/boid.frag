#version 430 core

struct Material
{
    sampler2D tex_diffuse0;
};

in vec2 UV;

uniform Material material;

out vec4 fColor;

void main()
{
    fColor = vec4(texture(material.tex_diffuse0, UV).rgb, 1.0f);
}