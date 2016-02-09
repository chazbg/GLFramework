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
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;
smooth in vec3 pos;
smooth in vec3 posV;
smooth in vec3 inTangent;
smooth in vec3 inBitangent;

// Ouput data
layout(location = 0) out vec3 outColor;

float rand(vec2 coords){
  return fract(sin(dot(coords.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// http://en.wikipedia.org/wiki/SRGB
vec3 linearToSRGB(vec3 linearRGB) {
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

// http://en.wikipedia.org/wiki/SRGB
vec3 sRGBToLinear(vec3 sRGB) {
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

////////////////////////////////////////////////////////////////////////////////
// Shading functions
////////////////////////////////////////////////////////////////////////////////

// Baked raw lighting map
vec3 getIncidentLighting() 
{
	return vec3(texture(colorMap, inUVs));
}

// IBL. Use HDR for best results.
vec3 getLightingFromDirection(vec3 vInDirection) 
{
	return vec3(texture(envMap, vInDirection));
}

// https://en.wikipedia.org/wiki/Fresnel_equations
float getFresnelReflectionWeight(vec3 vOutDirection, vec3 vNormal, float eta) {
	float cosThetaEnter = clamp(dot(vOutDirection, vNormal), 0.0, 1.0);
	float sinThetaEnter = sqrt(1.0 - cosThetaEnter * cosThetaEnter);
	float sinThetaExit = clamp(eta * sinThetaEnter, 0.0, 1.0);
	float cosThetaExit = sqrt(1.0 - sinThetaExit * sinThetaExit);

	float sReflectanceWeight = (eta * cosThetaEnter - cosThetaExit) / (eta * cosThetaEnter + cosThetaExit);
	float pReflectanceWeight = (eta * cosThetaExit - cosThetaEnter) / (eta * cosThetaExit + cosThetaEnter);
	float unpolarizedReflectionWeight = 0.5 * (sReflectanceWeight * sReflectanceWeight + pReflectanceWeight * pReflectanceWeight);

	return unpolarizedReflectionWeight;
}

////////////////////////////////////////////////////////////////////////////////
// GGX functions
// Burley, 2012, Physically-Based Shading at Disney, Section B.2
// Walter, 2007, Microfacet Models for Refraction through Rough Surfaces
////////////////////////////////////////////////////////////////////////////////

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

float GGXBRDFValue(vec3 vOutDirection, vec3 vNormal, vec3 vMicroNormal, vec3 vInDirection, float ior, float glossiness) {
	float F = getFresnelReflectionWeight(vOutDirection, vMicroNormal, ENVIRONMENT_IOR / ior);
	float G = GGXShadowingMasking(vOutDirection, vMicroNormal, vInDirection, glossiness);
	float D = GGXMicronormalDistribution(vNormal, vMicroNormal, glossiness);
	float dotNL = dot(vNormal, vInDirection);
	float dotNV = dot(vNormal, vOutDirection);
	
	return 0.25 * (F * G * D) / (dotNL * dotNV);
}

float radicalInverse_VdC(uint bits) {
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
 }
 // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
 vec2 Hammersley(uint i, uint N) {
     return vec2(float(i)/float(N), radicalInverse_VdC(i));
 }
 
 
vec3 importanceSampleGGX(vec2 xi, float roughness, vec3 n)
{
	float a = roughness * roughness;
	
	float phi = DOUBLE_PI * xi.x;
	float cosTheta = sqrt((1-xi.y)/(1 + (a*a - 1) * xi.y));
	float sinTheta = sqrt(1 - cosTheta * cosTheta);
	
	vec3 h = vec3(
		sinTheta * cos(phi),
		sinTheta * sin(phi),
		cosTheta
	);
	
	vec3 up = abs(n.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 tx = normalize(cross(up, n));
	vec3 ty = cross(n, tx);
	
	return tx * h.x + ty * h.y + n * h.z;
}
 
vec3 specularIBL(vec3 specularColor, float roughness, vec3 n, vec3 v)
{
	vec3 specLighting = vec3(0);
	
	const uint nSamples = 256u;
	for (uint i = 0u; i < nSamples; i++)
	{
		vec2 xi = Hammersley(i, nSamples);
		
		vec3 h = getGGXMicroNormal(xi.x, xi.y, roughness);//importanceSampleGGX(xi, roughness, n);
		vec3 l = 2 * dot(v, h) * h - v;
		
		float NoV = clamp(dot(n, v), 0, 1);
		float NoL = clamp(dot(n, l), 0, 1);
		float NoH = clamp(dot(n, h), 0, 1);
		float VoH = clamp(dot(v, h), 0, 1);
		
		if (NoL > 0)
		{
			vec3 sampleColor = getLightingFromDirection(l);
			
			float F = getFresnelReflectionWeight(v, h, ENVIRONMENT_IOR / ior);
			float G = GGXShadowingMasking(v, h, l, roughness);
			float D = GGXMicronormalDistribution(n, h, roughness);
			
			specLighting += sampleColor * F * G * VoH / (NoH * NoV);
		}
	}
	
	return specLighting / nSamples;
}

// Integrates the specular microfacet BRDF using Monter Carlo
vec3 getSpecularContribution(vec3 vOutDirection, vec3 vNormal, float ior, float glossiness) {
	vec3 specularContribution = vec3(0.0);
    
	vec2 randomSeed = posV.xy;
	
    const int SAMPLE_COUNT = 16;
	const float INVERSE_SAMPLE_COUNT = 1.0 / float(SAMPLE_COUNT);
	
    for (int sampleIndex = 0; sampleIndex < SAMPLE_COUNT; ++sampleIndex) {
		randomSeed = fract(randomSeed + INVERSE_SAMPLE_COUNT);
		// float xi1 = abs(snoise(vec3(randomSeed, 0))) + EPSILON;
		// float xi2 = abs(snoise(vec3(randomSeed.yx, 0))) + EPSILON;
		float xi1 = rand(randomSeed) + EPSILON;
		float xi2 = rand(randomSeed.yx) + EPSILON;
        
        // Generate light sample direction
		vec3 vSampleMicroNormal = getGGXMicroNormal(xi1, xi2, glossiness);
		vec3 vSampleReflection = reflect(-vOutDirection, vSampleMicroNormal);

        // Evaluate the integral argument
		float brdfValue = GGXBRDFValue(vOutDirection, vNormal, vSampleMicroNormal, vSampleReflection, ior, glossiness);
		vec3 incidentLight = getLightingFromDirection(vSampleReflection);
		float lambertTerm = dot(vSampleReflection, vNormal);
		
		// Evaluate the sample's probability
		float sampleProbability = GGXMicronormalDistribution(vNormal, vSampleMicroNormal, glossiness) * dot(vNormal, vSampleMicroNormal);
		
        specularContribution += ((brdfValue * incidentLight * lambertTerm) / sampleProbability);
    }
	
    specularContribution *= INVERSE_SAMPLE_COUNT;
	
	return specularContribution;
}

vec3 getSpecularContribution2(vec3 vOutDirection, vec3 vNormal, vec3 l, float ior, float glossiness) {
	vec3 specularContribution = vec3(0.0);

    // Generate light sample direction
    vec3 vSampleMicroNormal = normalize(vOutDirection + l);
    vec3 vSampleReflection = reflect(-vOutDirection, vSampleMicroNormal);

    // Evaluate the integral argument
    float brdfValue = GGXBRDFValue(vOutDirection, vNormal, vSampleMicroNormal, vSampleReflection, ior, glossiness);
    vec3 incidentLight = getLightingFromDirection(vSampleReflection);
    float lambertTerm = dot(vSampleReflection, vNormal);
    
    // Evaluate the sample's probability
    float sampleProbability = GGXMicronormalDistribution(vNormal, vSampleMicroNormal, glossiness) * dot(vNormal, vSampleMicroNormal);
    
    specularContribution += ((brdfValue * incidentLight * lambertTerm) / sampleProbability);
	
	return specularContribution;
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
    diffuseContribution *= diffuse;
	
    lightSampleValues light0 = computePointLightValues(light0Pos, vec3(0,0,1), 4, pos);
    lightSampleValues light1 = computePointLightValues(light1Pos, vec3(0,0,1), 4, pos);
    lightSampleValues light2 = computePointLightValues(light2Pos, vec3(0,0,1), 8, pos);
    
	// Calculate the specular reflection weight
    vec3 vOutDirection = normalize(cameraPos - pos);
    float specularWeight = getFresnelReflectionWeight(vOutDirection, normalize(inNormal), ENVIRONMENT_IOR / ior);
    
	// Calculate the specular contribution
    //vec3 specularContribution = getSpecularContribution(vOutDirection, normalize(inNormal), ior, 1.0 - glossiness);
    
    // specularContribution += getSpecularContribution2(vOutDirection, normalize(inNormal), light0.L, ior, 1.0 - glossiness);
    // specularContribution += getSpecularContribution2(vOutDirection, normalize(inNormal), light1.L, ior, 1.0 - glossiness);
    // specularContribution += getSpecularContribution2(vOutDirection, normalize(inNormal), light2.L, ior, 1.0 - glossiness);
    
    //specularContribution *= specular;
	
	vec3 specularContribution = specularIBL(specular, 1.0 - glossiness, normalize(inNormal), vOutDirection);
    // Energy preservation
    vec3 result = mix(diffuseContribution, specularContribution, specularWeight);
    
	// Convert to sRGB    
    outColor = linearToSRGB(result);
    
    //outColor = vec3(0.5 * normalize(inNormal) + 0.5);
    //outColor = vec3(0.5 * normalize(inTangent) + 0.5);
    //outColor = vec3(0.5 * normalize(inBitangent) + 0.5);
}