#version 430 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vSpeed;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

out vec3 speed;

void main()
{
    vec3 fragPos = vec3(model * vec4(vPos.xyz, 1.0));

    float distCamera = length(fragPos - viewPos);

    float minDist = 3.0;
    float maxDist = 40.0;
    float minSize = 1.0;
    float maxSize = 10.0;

    float t = clamp((distCamera - minDist) / (maxDist - minDist), 0.0, 1.0);
    gl_PointSize = mix(maxSize, minSize, t);

    gl_Position = projection * view * vec4(fragPos, 1.0);

    speed = vSpeed.xyz;
}
