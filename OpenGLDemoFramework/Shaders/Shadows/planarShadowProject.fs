#version 330 core

uniform vec3 diffuse;

// Ouput data
layout(location = 0) out vec4 outColor;

in float projectedDistance;

void main()
{
    outColor = vec4(vec3(0.2), projectedDistance);
}