#version 330 core

// Useful math constants
const float PI = 3.14159265;
const float DOUBLE_PI = 6.2831853;
const float INVERSE_PI = 0.318309886;
const float EPSILON = 0.001;

// Settings constants
const float ENVIRONMENT_IOR = 1.0;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D aoMap;
uniform sampler2D reflectionMap;
uniform sampler2D illuminationMap;

uniform vec3 cameraPos;
uniform mat4 mv;

// Input data
smooth in vec3 inNormal;
smooth in vec3 inTangent;
smooth in vec3 inBitangent;
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;
smooth in vec3 pos;

// Ouput data
layout(location = 0) out vec3 outColor;

vec3 linearToSRGB(vec3 linearRGB) 
{
    const vec3 INVERSE_GAMMA = vec3(1.0 / 2.4);
    const vec3 K0 = vec3(0.0031308);
    const float ALPHA = 0.055;
    const float PHI = 12.92;
    
    bvec3 lessThanK0 = lessThanEqual(linearRGB, K0);
    
    vec3 sRGBLessThanK0 = PHI * linearRGB;
    vec3 sRGBLargerThanK0 = (1.0 + ALPHA) * pow(linearRGB, vec3(INVERSE_GAMMA)) - ALPHA;
    
    vec3 sRGB = mix(sRGBLargerThanK0, sRGBLessThanK0, vec3(lessThanK0));
    return sRGB;
}

vec3 sRGBToLinear(vec3 sRGB) 
{
    const vec3 GAMMA = vec3(2.4);
    const vec3 K0 = vec3(0.04045);
    const float ALPHA = 0.055;
    const float INVERSE_PHI = 1.0 / 12.92;
    
    bvec3 lessThanK0 = lessThanEqual(sRGB, K0);
    
    vec3 linearRGBLessThanK0 = INVERSE_PHI * sRGB;
    vec3 linearRGBLargerThanK0 = pow((sRGB + ALPHA) / (1.0 + ALPHA), GAMMA);
	
    vec3 linearRGB = mix(linearRGBLargerThanK0, linearRGBLessThanK0, vec3(lessThanK0));
    return linearRGB;
}

float getDiffuseContribution(vec3 n, vec3 l)
{
    float NoL = max(0, dot(n, l));
    
    return NoL * INVERSE_PI;
}

float getSpecularContribution(vec3 n, vec3 v, vec3 l, float m)
{
    vec3 h = normalize(v + l);
    float NoH = max(0.0, dot(n, h));
    return (m + 8) * pow(NoH, m) * 0.125 * INVERSE_PI;
}

vec3 getNormal(vec2 d)
{
    return normalize(normalize(inNormal) + normalize(inTangent) * d.x + normalize(inBitangent) * d.y);
}

void main()
{
    vec2 uv           = vec2(inUVs.x, 1.0 - inUVs.y);
    vec3 diffuse      = texture(diffuseMap, uv).bgr;
    float glossiness  = texture(specularMap, uv).r;
    vec3 n            = getNormal(texture(normalMap, uv).ra * 2.0 - 1.0);
    float ao          = texture(aoMap, uv).r;
    //float reflection  = texture(reflectionMap, uv).r;
    //vec3 illumination = texture(illuminationMap, uv);
    
    vec3 l = normalize(vec3(1.0));
    float lightIntensity = 1.0;
    
    vec3 v = normalize(cameraPos - pos);
    //vec3 n = normalize(inNormal);
    
    float m = glossiness * 16.0;
    
    float diffuseContribution = 0;
    
    diffuseContribution += getDiffuseContribution(n, l) * lightIntensity;
    
	float specularContribution = 0;
    
    specularContribution += getSpecularContribution(n, v, l, m) * lightIntensity;
    vec3 result = diffuseContribution * diffuse * ao + specularContribution;
    
	// Convert to sRGB    
    outColor = linearToSRGB(result);
    
    //outColor = vec3(0.5 * normalize(inNormal) + 0.5);
    //outColor = vec3(0.5 * normalize(inTangent) + 0.5);
    //outColor = vec3(0.5 * normalize(inBitangent) + 0.5);
}