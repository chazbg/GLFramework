#version 330 core

// Input data
smooth in vec4 inColor;
smooth in vec2 inTexCoords;

uniform sampler2D sampler;

// Ouput data
out vec3 outColor;

void main()
{
    // outColor = inColor;
    float i = texture2D(sampler, inTexCoords).x;
    outColor = mix(vec3(0, 0, 1), vec3(1, 1, 1), i);
}