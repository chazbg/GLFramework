#version 330 core

uniform vec3 diffuse;

// Ouput data
layout(location = 0) out vec3 outColor;

void main()
{
    outColor = diffuse;
}