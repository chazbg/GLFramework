#version 330 core

// Useful math constants
const float PI = 3.14159265;
const float DOUBLE_PI = 6.2831853;
const float INVERSE_PI = 0.318309886;
const float EPSILON = 0.001;

// Settings constants
const float ENVIRONMENT_IOR = 1.0;

uniform sampler2D colorMap;
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

float radicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
 
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
vec2 Hammersley(uint i, uint N) 
{
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}
 
vec3 getIncidentLighting() 
{
	return vec3(texture(colorMap, inUVs));
}

// IBL. Use HDR for best results.
vec3 getLightingFromDirection(vec3 vInDirection) 
{
	return vec3(texture(envMap, vInDirection)).bgr;
}

vec3 getGGXMicroNormal(float xi1, float xi2, float glossiness) {
	float phi = DOUBLE_PI * xi1;
	float tanTheta = sqrt(xi2 / (1.0 - xi2));
	
	vec3 microNormal = 
		tanTheta * glossiness * cos(phi) * normalize(inTangent) +
		tanTheta * glossiness * sin(phi) * normalize(inBitangent) +
		normalize(inNormal);
		
	microNormal = normalize(microNormal);
	return microNormal;
}

float GGXShadowingMasking(vec3 vOutDirection, vec3 vMicroNormal, vec3 vInDirection, float glossiness) {
	float glossinessSquared = glossiness * glossiness;
	
	float cosThetaL = dot(vInDirection, vMicroNormal);
	float cosThetaLSquared = cosThetaL * cosThetaL;
	float tanThetaLSquared = 1.0 / cosThetaLSquared - 1.0;
	float shadowing = 2.0 / (1.0 + sqrt(1.0 + glossinessSquared * tanThetaLSquared));
	
	float cosThetaV = dot(vOutDirection, vMicroNormal);
	float cosThetaVSquared = cosThetaV * cosThetaV;
	float tanThetaVSquared = 1.0 / cosThetaVSquared - 1.0;
	float masking = 2.0 / (1.0 + sqrt(1.0 + glossinessSquared * tanThetaVSquared));
	
	return shadowing * masking;
}

float GGXMicronormalDistribution(vec3 vNormal, vec3 vMicroNormal, float glossiness) {
	float cosThetaM = dot(vNormal, vMicroNormal);
	float cosThetaMSquared = cosThetaM * cosThetaM;
	float sinThetaMSquared = 1.0 - cosThetaMSquared;
	float glossinessSquared = glossiness * glossiness;
	
	float num = INVERSE_PI * glossinessSquared;
	float sqrtDen = (sinThetaMSquared + glossinessSquared * cosThetaMSquared);
	
	return num / (sqrtDen * sqrtDen);
}
 
vec3 specularIBL(vec3 specularColor, float roughness, vec3 n, vec3 v, out float NoL)
{
	vec3 specLighting = vec3(0);
	
	const uint nSamples = 16u;
	for (uint i = 0u; i < nSamples; i++)
	{
		vec2 xi = Hammersley(i, nSamples);
		
		vec3 h = getGGXMicroNormal(xi.x, xi.y, roughness);
		vec3 l = 2 * dot(v, h) * h - v;
		
		float NoV = clamp(dot(n, v), 0, 1);
              NoL = clamp(dot(n, l), 0, 1);
		float NoH = clamp(dot(n, h), 0, 1);
		float VoH = clamp(dot(v, h), 0, 1);
		
		if (NoL > 0)
		{
			vec3 sampleColor = getLightingFromDirection(l);
			
			float Fc = pow( 1 - VoH, 5);
            vec3 F = (1 - Fc) * specularColor + Fc;
			float G = GGXShadowingMasking(v, h, l, roughness);
			float D = GGXMicronormalDistribution(n, h, roughness);
			
			specLighting += sampleColor * F * G * VoH / (NoH * NoV);
		}
	}
	
	return specLighting / nSamples;
}

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

void main()
{
    vec3 diffuseContribution = getIncidentLighting();
    diffuseContribution *= diffuse * INVERSE_PI;
	
    lightSampleValues light0 = computePointLightValues(light0Pos, vec3(0,0,1), 4, pos);
    lightSampleValues light1 = computePointLightValues(light1Pos, vec3(0,0,1), 4, pos);
    lightSampleValues light2 = computePointLightValues(light2Pos, vec3(0,0,1), 8, pos);
    
	// Calculate the specular reflection weight
    vec3 vOutDirection = normalize(cameraPos - pos);
    float specularWeight = ior;
    
	// Calculate the specular contribution
    //vec3 specularContribution = getSpecularContribution(vOutDirection, normalize(inNormal), ior, 1.0 - glossiness);
    
    // specularContribution += getSpecularContribution2(vOutDirection, normalize(inNormal), light0.L, ior, 1.0 - glossiness);
    // specularContribution += getSpecularContribution2(vOutDirection, normalize(inNormal), light1.L, ior, 1.0 - glossiness);
    // specularContribution += getSpecularContribution2(vOutDirection, normalize(inNormal), light2.L, ior, 1.0 - glossiness);
    
    //specularContribution *= specular;
	
    float NoL;
	vec3 specularContribution = specularIBL(specular, 1.0 - glossiness, normalize(inNormal), vOutDirection, NoL);
    // Energy preservation
    vec3 result = mix(diffuseContribution, specularContribution, specularWeight);
    
	// Convert to sRGB    
    outColor = linearToSRGB(result);
    
    //outColor = vec3(0.5 * normalize(inNormal) + 0.5);
    //outColor = vec3(0.5 * normalize(inTangent) + 0.5);
    //outColor = vec3(0.5 * normalize(inBitangent) + 0.5);
}