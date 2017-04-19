#version 330 core

// Input data
smooth in vec3 vNormal;
smooth in vec3 vPos;
smooth in vec2 vUV;

// Ouput data
out vec3 outColor;

uniform vec3  diffuse;
uniform vec3  pointLightPos;
uniform vec3  dirLightDirection;
uniform vec3  spotLightPos;
uniform vec3  spotLightDir;
uniform float spotLightAngle;
uniform sampler2D shadowTexture;

#define SAMPLES 4

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

float getGaussianCoefficient(vec2 dist, float strength)
{
    return 1.0 / (2.0 * 3.14 * strength * strength) * pow(2.71828, -dot(dist, dist) / (2.0 * strength * strength));
}

vec3 blurTexture(sampler2D tex, vec2 uv, float strength)
{
    vec3 c = vec3(0.0);
    vec2 delta = 1.0 / vec2(1600.0, 900.0);
    for (float i = -SAMPLES * 0.5; i < SAMPLES * 0.5; i += 1.0)
    {
        for (float j = -SAMPLES * 0.5; j < SAMPLES * 0.5; j += 1.0)
        {
            vec2 dist = vec2(i, j) * delta;
            c += texture(shadowTexture, vUV + dist).rgb * getGaussianCoefficient(dist, strength);
        }
    }
    return c;
}

void main()
{
    vec3 n   = normalize(vNormal);
    float c1 = getPointLightContribution(n, vPos, pointLightPos, 32.0);
    float c2 = getDirLightContribution(n, dirLightDirection, 0.25);
    float c3 = getSpotlightContribution(n, vPos, spotLightPos, spotLightDir, spotLightAngle, 16.0);
    float projectedDistance = texture(shadowTexture, vUV).a;
    //vec3 c = blurTexture(shadowTexture, vUV, projectedDistance * 5.0);
    vec3 c = texture(shadowTexture, vUV).rgb;
    vec3 shadowColor = c;
    outColor = diffuse * (c1 + c2 + c3) - shadowColor;
    //outColor = vec3(projectedDistance);
}