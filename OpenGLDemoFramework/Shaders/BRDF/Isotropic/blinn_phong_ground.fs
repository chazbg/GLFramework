#version 330 core

// Useful math constants
const float PI = 3.14159265;
const float DOUBLE_PI = 6.2831853;
const float INVERSE_PI = 0.318309886;
const float EPSILON = 0.001;

// Settings constants
const float ENVIRONMENT_IOR = 1.0;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specMap;
uniform sampler2D sampler;
uniform samplerCube envMap;

uniform float time;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 cameraPos;
uniform float ior;
uniform float glossiness;
uniform vec3 light0Pos;
uniform vec3 light1Pos;
uniform vec3 light2Pos;
uniform mat4 mv;

// Input data
smooth in vec3 inNormal;
smooth in vec3 inTangent;
smooth in vec3 inBitangent;
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;
smooth in vec3 pos;
smooth in vec3 posV;

// Ouput data
layout(location = 0) out vec3 outColor;

struct lightSampleValues {
	vec3 L;
	float iL;
};

lightSampleValues computePointLightValues(vec3 pointLightPosition, vec3 pointLightAttenuation, float pointLightIntensity, vec3 surfacePosition)
{
	lightSampleValues values;
	values.L = pointLightPosition - surfacePosition;
	float dist = length(values.L);
	values.L = values.L / dist; // normalize
	// Dot computes the 3-term attenuation in one operation
	// k_c * 1.0 + k_l * dist + k_q * dist * dist
	float distAtten = dot(pointLightAttenuation, vec3(1.0, dist, dist*dist));
	values.iL = pointLightIntensity / distAtten;
	return values;
}

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

void main()
{
    vec3 diffuseContribution = vec3(texture(diffuseMap, inUVs).bgr);
    float gloss = length(texture(specMap, inUVs));
    
    lightSampleValues light0 = computePointLightValues(light0Pos, vec3(0,0,1), 64, pos);
    lightSampleValues light1 = computePointLightValues(light1Pos, vec3(0,0,1), 64, pos);
    lightSampleValues light2 = computePointLightValues(light2Pos, vec3(0,0,1), 128, pos);
    
    vec3 v = normalize(cameraPos - pos);
    vec3 texNormal = 2.0 * texture(normalMap, inUVs).bgr - 1;
    mat3 tr = mat3(normalize(inTangent), normalize(inBitangent), normalize(inNormal));
    vec3 n = vec3(tr * texNormal);
    vec3 r = normalize(reflect(-v, n));
    vec3 h0 = normalize(light0.L + v);
    vec3 h1 = normalize(light1.L + v);
    vec3 h2 = normalize(light2.L + v);
    
    float NoH0 = max(0.0, dot(n, h0));
    float NoH1 = max(0.0, dot(n, h1));
    float NoH2 = max(0.0, dot(n, h2));
    
    float specularWeight = 1 - ior;
    
    float m = texture(specMap, inUVs).r * 255;
    
	vec3 specularContribution  = vec3(0);
    
    specularContribution += specular * (m + 8) * pow(NoH0, m) * light0.iL;
    specularContribution += specular * (m + 8) * pow(NoH1, m) * light1.iL;
    specularContribution += specular * (m + 8) * pow(NoH2, m) * light2.iL;
    
    vec3 result = diffuseContribution * INVERSE_PI + specularContribution * 0.125 * INVERSE_PI;
    
	// Convert to sRGB    
    outColor = linearToSRGB(result);
    
    //outColor = vec3(0.5 * normalize(inNormal) + 0.5);
    //outColor = vec3(0.5 * normalize(inTangent) + 0.5);
    //outColor = vec3(0.5 * normalize(inBitangent) + 0.5);
}