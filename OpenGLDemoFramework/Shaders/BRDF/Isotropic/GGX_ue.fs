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

struct lightSampleValues {
	vec3 L;
	float iL;
};

lightSampleValues computePointLightValues(vec3 pointLightPosition, vec3 pointLightAttenuation, float pointLightIntensity, vec3 surfacePosition, vec3 r)
{
	lightSampleValues values;
	values.L = pointLightPosition - surfacePosition;
    float LoR = dot(values.L, r);
    
    if (LoR > 0)
    {
        vec3 centerToRay = LoR * r - values.L;
        float centerToRayLength = length(centerToRay);
        vec3 closestPoint = values.L + centerToRay * clamp(2 / centerToRayLength, 0, 1);
        float dist = length(closestPoint);
        
        // Dot computes the 3-term attenuation in one operation
        // k_c * 1.0 + k_l * dist + k_q * dist * dist
        float distAtten = dot(pointLightAttenuation, vec3(1.0, dist, dist*dist));

        values.iL = (pointLightIntensity / distAtten) * max(0, 2 - centerToRayLength);
	}
    
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

vec3 getLightingFromDirection(vec3 vInDirection) 
{
	vec3 c = textureLod(envMap, vInDirection, (1 - glossiness * glossiness) * 8).bgr;
	//vec3 c = texture(envMap, vInDirection).bgr;
    
	lightSampleValues light0 = computePointLightValues(light0Pos, vec3(0,0,1), 128, pos, vInDirection);
    lightSampleValues light1 = computePointLightValues(light1Pos, vec3(0,0,1), 128, pos, vInDirection);
    lightSampleValues light2 = computePointLightValues(light2Pos, vec3(0,0,1), 128, pos, vInDirection);
	
	c = c + vec3(1,0,0) * light0.iL + 
			vec3(0,1,0) * light1.iL + 
			vec3(0,0,1) * light2.iL;
			
	return c;
}

float G1(vec3 n, vec3 v, float k)
{
    float NoV = clamp(dot(n, v), 0, 1);
    return NoV/(NoV * (1 - k) + k);
}

float G_Smith(vec3 vOutDirection, vec3 vMicroNormal, vec3 vInDirection, float roughness) {
    float k = pow((roughness + 1), 2) / 8;
    
	return G1(vMicroNormal, vInDirection, k) *
           G1(vMicroNormal, vOutDirection, k);
}

vec3 ImportanceSampleGGX(vec2 Xi, float Roughness, vec3 N)
{
    float a = Roughness * Roughness;
    float Phi = 2 * PI * Xi.x;
    float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
    float SinTheta = sqrt(1 - CosTheta * CosTheta);
    vec3 H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;
    
    vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
    vec3 TangentX = normalize(cross(UpVector , N));
    vec3 TangentY = cross(N, TangentX);
    
    // Tangent to world space
    return TangentX * H.x + TangentY * H.y + N * H.z;
}

float schlick(float specular, vec3 v, vec3 h)
{
	float VoH = dot(v, h);
	float Fc = pow(1 - VoH, 5);
	float F = (1 - Fc) * specular + Fc;
	return F; 
}

vec3 specularIBL(vec3 specularColor, float roughness, vec3 n, vec3 v, out float dotNL, float ior)
{
	vec3 specLighting = vec3(0);
	dotNL = 0;
	const uint nSamples = 64u;
	for (uint i = 0u; i < nSamples; i++)
	{
		vec2 xi = Hammersley(i, nSamples);
		
        vec3 h = normalize(ImportanceSampleGGX(xi, roughness, n));
		vec3 l = 2 * dot(v, h) * h - v;
		
		float NoV = clamp(dot(n, v), 0, 1);
        float NoL = clamp(dot(n, l), 0, 1);
		float NoH = clamp(dot(n, h), 0, 1);
		float VoH = clamp(dot(v, h), 0, 1);
		
		if (NoL > 0)
		{
			vec3 sampleColor = getLightingFromDirection(l);
			
            float F = schlick(1 - ior, v, h);
			float G = G_Smith(v, h, l, roughness);

			specLighting += sampleColor * F * G * VoH / (NoH * NoV);
            
            dotNL += NoL;
		}
	}
	
    dotNL /= nSamples;
	return specLighting / nSamples;
}

void main()
{
	// Calculate the specular reflection weight
    vec3 vOutDirection = normalize(cameraPos - pos);
    float specularWeight = 1 - ior;
	
    float NoL;
	vec3 specularContribution = specularIBL(specular, 1.0 - glossiness, normalize(inNormal), vOutDirection, NoL, ior);
    vec3 diffuseContribution = diffuse * INVERSE_PI * NoL;
    
    // Energy preservation
    vec3 result = mix(diffuseContribution, specularContribution, specularWeight);
    
	// Convert to sRGB    
    outColor = linearToSRGB(result);
    
    //outColor = vec3(0.5 * normalize(inNormal) + 0.5);
    //outColor = vec3(0.5 * normalize(inTangent) + 0.5);
    //outColor = vec3(0.5 * normalize(inBitangent) + 0.5);
}