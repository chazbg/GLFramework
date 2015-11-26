#version 330 core

// Input data
smooth in vec4 n;

// Ouput data
layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;

void main()
{
    outColor = vec3(1.0, 0.0, 0.0);
    outNormal = n.xyz;
}