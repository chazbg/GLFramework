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

float getMaxDistanceAtRow(sampler2D tex, vec2 uv, float row)
{
    vec2 delta = 1.0 / vec2(1600.0, 900.0);
    float d = 0.0;
    
    d = max(d, texture(tex, uv + vec2(-2.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2(-1.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2( 0.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2( 1.0 * delta.x, row * delta.y)).a);
    d = max(d, texture(tex, uv + vec2( 2.0 * delta.x, row * delta.y)).a);
    
    return d;
}

float getDistance(sampler2D tex, vec2 uv)
{
    float d = 0.0;
    
    d = max(d, getMaxDistanceAtRow(tex, uv, -2.0));
    d = max(d, getMaxDistanceAtRow(tex, uv, -1.0));
    d = max(d, getMaxDistanceAtRow(tex, uv,  0.0));
    d = max(d, getMaxDistanceAtRow(tex, uv,  1.0));
    d = max(d, getMaxDistanceAtRow(tex, uv,  2.0));
    
    return d * 10.0;
}

vec3 blurColor(sampler2D tex, vec2 uv, float strength)
{
    vec2 delta = 1.0 / vec2(1600.0, 900.0) * strength;
    vec3 c = texture(shadowTexture, vUV).rgb * 41.0 / 273.0;
    
    c += texture(tex, uv + vec2(delta.x, 0.0)).rgb  * 26.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, delta.y)).rgb  * 26.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, 0.0)).rgb * 26.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, -delta.y)).rgb * 26.0 / 273.0;
                 
    c += texture(tex, uv + vec2(delta.x, delta.y)).rgb   * 16.0 / 273.0;
    c += texture(tex, uv + vec2(delta.x, -delta.y)).rgb  * 16.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, delta.y)).rgb  * 16.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, -delta.y)).rgb * 16.0 / 273.0;
                 
    c += texture(tex, uv + vec2(2.0 * delta.x, 0.0)).rgb  * 7.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, 2.0 * delta.y)).rgb  * 7.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, 0.0)).rgb * 7.0 / 273.0;
    c += texture(tex, uv + vec2(0.0, 2.0 * -delta.y)).rgb * 7.0 / 273.0;
                 
    c += texture(tex, uv + vec2(2.0 * delta.x, delta.y)).rgb   * 4.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * delta.x, -delta.y)).rgb  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, delta.y)).rgb  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, -delta.y)).rgb * 4.0 / 273.0;
    c += texture(tex, uv + vec2(delta.x,  2.0 * delta.y)).rgb  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(delta.x,  2.0 * -delta.y)).rgb * 4.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, 2.0 * delta.y)).rgb  * 4.0 / 273.0;
    c += texture(tex, uv + vec2(-delta.x, 2.0 * -delta.y)).rgb * 4.0 / 273.0;
                 
    c += texture(tex, uv + vec2(2.0 * delta.x, 2.0 * delta.y)).rgb   * 1.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * delta.x, 2.0 * -delta.y)).rgb  * 1.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, 2.0 * delta.y)).rgb  * 1.0 / 273.0;
    c += texture(tex, uv + vec2(2.0 * -delta.x, 2.0 * -delta.y)).rgb * 1.0 / 273.0;
    
    return c;
}

void main()
{
    vec3 n   = normalize(vNormal);
    float c1 = getPointLightContribution(n, vPos, pointLightPos, 32.0);
    float c2 = getDirLightContribution(n, dirLightDirection, 0.25);
    float c3 = getSpotlightContribution(n, vPos, spotLightPos, spotLightDir, spotLightAngle, 16.0);
    
    float projectedDistance = getDistance(shadowTexture, vUV);
    vec3  shadowColor       = 1.0 - blurColor(shadowTexture, vUV, projectedDistance);
    
    outColor = diffuse * (c1 + c2 + c3) * shadowColor;
}