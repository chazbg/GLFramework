#version 330 core

// Input data
smooth in vec3 pos;
smooth in vec3 inNormal;
smooth in vec3 inTangent;
smooth in vec3 inBitangent;
smooth in vec2 inUVs;

// Ouput data
layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;

void main()
{
    outColor  = normalize(pos);
    outNormal = normalize(inNormal);
}