#version 330 core

uniform samplerCube envMap;

// Input data
smooth in vec3 iPos;

// Ouput data
layout(location = 0) out vec3 outColor;

void main()
{
    outColor = texture(envMap, iPos).rgb;
}