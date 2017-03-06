#version 330 core

// Input data
in vec3 inColor;
in vec2 inTexCoords;

uniform sampler2D sampler;

// Ouput data
out vec3 outColor;

void main()
{
    outColor = inColor;
}