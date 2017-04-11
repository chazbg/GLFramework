#version 330 core

// Input data
smooth in vec3 vNormal;
smooth in vec3 vPos;

// Ouput data
out vec3 outColor;

uniform vec3  diffuse;
uniform vec3  pointLightPos;
uniform vec3  dirLightDirection;
uniform vec3  spotLightPos;
uniform vec3  spotLightDir;
uniform float spotLightAngle;

float getPointLightContribution(vec3 n, vec3 pos, vec3 lightPos, float intensity)
{
    vec3  ray              = pos - lightPos;
    float rayLengthSquared = dot(ray, ray);
    vec3  l                = -ray / sqrt(rayLengthSquared);
    return max(0.0, dot(n, l)) * intensity / rayLengthSquared;
}

float getDirLightContribution(vec3 n, vec3 lightDir, float intensity)
{
    return max(0.0, dot(n, -lightDir)) * intensity;
}

float getSpotlightContribution(vec3 n, vec3 pos, vec3 lightPos, vec3 lightDir, float angle, float intensity)
{
    vec3  ray              = pos - lightPos;
    float rayLengthSquared = dot(ray, ray);
    vec3  l                = -ray / sqrt(rayLengthSquared);
    
    if (cos(angle) > dot(lightDir, -l))
    {
        return 0.0;
    }
    else
    {
        return max(0.0, dot(n, l)) * intensity / rayLengthSquared;
    }
}

void main()
{
    vec3 n   = normalize(vNormal);
    float c1 = getPointLightContribution(n, vPos, pointLightPos, 64.0);
    float c2 = getDirLightContribution(n, dirLightDirection, 0.25);
    float c3 = getSpotlightContribution(n, vPos, spotLightPos, spotLightDir, spotLightAngle, 16.0);
    
    outColor = diffuse * (c1 + c2 + c3);
}