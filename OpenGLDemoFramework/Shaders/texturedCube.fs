#version 330 core

uniform sampler2D colorMap;
uniform sampler2D sampler;
uniform samplerCube cubeMap;

uniform float time;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 cameraPos;
uniform float ior;
uniform float glossiness;
uniform vec3 light0Pos;
uniform vec3 light1Pos;
uniform vec3 light2Pos;

// Input data
smooth in vec3 inNormal;
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;
smooth in vec3 pos;

// Ouput data
layout(location = 0) out vec3 outColor;

struct lightSampleValues {
	vec3 L;
	float iL;
};

float linearToSRGB(float c)
{
   if (c > 1.0)
      return 1.0;
   else if (c < 0.0)
      return 0.0;
   else if (c < 0.0031308)
      return 12.92 * c;
   else
      return 1.055 * pow(c, 0.41666) - 0.055;
}

vec3 linearToSRGB(vec3 c)
{
	return vec3(
		linearToSRGB(c.r),
		linearToSRGB(c.g),
		linearToSRGB(c.b)
	);
}

vec3 computeAmbientComponent(lightSampleValues light, vec3 materialAmbientColor, vec3 lightAmbDiffSpec, vec3 lightColor)
{
	return light.iL * (lightColor * lightAmbDiffSpec.x) * materialAmbientColor;
}

// surfaceNormal is assumed to be unit-length
vec3 computeDiffuseComponent(vec3 surfaceNormal, lightSampleValues light, vec3 materialDiffuseColor, vec3 lightAmbDiffSpec, vec3 lightColor)
{
	return light.iL * (lightColor * lightAmbDiffSpec.y)
					* materialDiffuseColor.rgb
					* max(0.0, dot(surfaceNormal, light.L));
}

vec3 computeSpecularComponent(vec3 surfaceNormal, vec3 surfacePosition, lightSampleValues light, vec3 materialSpecularColor, float materialSpecularExp, vec3 lightAmbDiffSpec, vec3 lightColor, vec3 cameraPos)
{
	vec3 viewVector = normalize(cameraPos - surfacePosition);
	vec3 reflectionVector = 2.0 * dot(light.L, surfaceNormal) * surfaceNormal - light.L;
	return (dot(surfaceNormal, light.L) <= 0.0) ? vec3(0.0,0.0,0.0) : (light.iL * (lightColor * lightAmbDiffSpec.z)
																				* materialSpecularColor
																				* pow(max(0.0, dot(reflectionVector, viewVector)), materialSpecularExp));
}

lightSampleValues computeDirLightValues(vec3 dirLightPosition, float dirLightIntensity)
{
	lightSampleValues values;
	values.L = dirLightPosition.xyz;
	values.iL = dirLightIntensity;
	return values;
}

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

float rf0(float ior)
{
    return pow((ior-1)/(ior+1),2);
}

float rf(float cosTheta)
{
    return rf0(ior) + (1 - rf0(ior))*pow((1 - cosTheta), 5);
}

vec3 F(vec3 l, vec3 h)
{
    vec3 cSpec = vec3(1,0.71,0.29);
    return cSpec + (1-cSpec)*pow(1-dot(l,h), 5);
}

float G(vec3 l, vec3 v, vec3 h, vec3 n)
{
    return min(1, min(2*dot(n,h)*dot(n,v)/dot(v,h), 2*dot(n,h)*dot(n,l)/dot(v,h)));
}

float NDF(vec3 n, vec3 m, float glossiness)
{
    const int SAMPLES = 16;
    const float INV_SAMPLES = 1.0 / 16.0;
    
    float glossinessSquared = glossiness * glossiness;
    float dotNM = dot(n, m);
    float dotNMSquared = dotNM * dotNM;
    float res = glossinessSquared / (3.14 * pow(dotNMSquared * (glossinessSquared - 1) + 1, 2));
    return res;
}

vec3 f(vec3 l, vec3 v, vec3 n)
{
    vec3 h = normalize(v + l);
    return F(l,h)*G(l,v,h,n)*NDF(n, h, glossiness)/(4*dot(n,l)*dot(n,v));
}

void main()
{
	vec3 ambColor = vec3(1, 1, 1);
	vec3 emissiveColor = vec3(1,1,1);
	vec3 diffuseColor = vec3(1,1,0);
	vec3 specularColor = vec3(1,1,1);
	vec3 lightAmbDiffSpec = vec3(0.02, (1.0 - ior) * 0.98, ior * 0.98);
	vec3 lightColor = vec3(1,1,1);
	float specExp = 6.0;
    
    lightSampleValues light0 = computePointLightValues(light0Pos, vec3(0,0,1), 4, pos);
    lightSampleValues light1 = computePointLightValues(light1Pos, vec3(0,0,1), 4, pos);
    lightSampleValues light2 = computePointLightValues(light2Pos, vec3(0,0,1), 8, pos);
    
    vec3 n = normalize(inNormal);    
    vec3 v = normalize(cameraPos - pos);

    vec3 cl0 = f(light0.L, v, n)*max(0,dot(light0.L, n));
    vec3 cl1 = f(light1.L, v, n)*max(0,dot(light1.L, n));
    vec3 cl2 = f(light2.L, v, n)*max(0,dot(light2.L, n));
    
    outColor = cl0 + cl1 + cl2;
}