#version 330 core

uniform vec3 diffuse;

// Ouput data
layout(location = 0) out vec4 outColor;

uniform vec3  spotLightPos;
uniform vec3  spotLightDir;
uniform float spotLightAngle;

in vec3 pos;
in float projectedDistance;

void main()
{
    vec3 ray = normalize(pos - spotLightPos);
    if (cos(spotLightAngle) < dot(ray, spotLightDir))
    {
        outColor = vec4(vec3(0.2), projectedDistance);
    }
    else
    {
        outColor = vec4(vec3(0.0), projectedDistance);
    }
}