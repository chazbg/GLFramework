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
smooth in vec3 posV;
smooth in vec3 inTangent;
smooth in vec3 inBitangent;

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
    float glossinessSquared = glossiness * glossiness;
    float dotNM = dot(n, m);
    float dotNMSquared = dotNM * dotNM;
    float res = glossinessSquared / (3.14 * pow(dotNMSquared * (glossinessSquared - 1) + 1, 2));
    return res;
}

vec3 f(vec3 l, vec3 v, vec3 n)
{
    vec3 dir = normalize(inTangent);//vec3(1,0,0);
    const float m = 16;
    float dotLT = dot(l, dir);
    float dotVT = dot(v, dir);
    vec3 projV = normalize(dir * dotVT + v);
    vec3 projL = normalize(dir * dotLT + l);
    //float cosAr = max(0, sqrt(1 - dotLT*dotLT)*sqrt(1 - dotVT*dotVT) - dotLT*dotVT);
    //return diffuse/3.14 + (m+2)/(2*3.14)*specular*pow(cosAr, m);
    
    vec3 h = normalize(v + l);
    return F(l,h)*G(l,v,h,projV)*NDF(projV, h, glossiness)/(4*dot(projV,l)*dot(projV,v));
    
    // float nu = 5;
    // float nv = 5;
    // float dotNH = dot(n, h);
    // float dotHL = dot(h, l);
    // float dotNL = dot(n, l);
    // float dotNV = dot(n, v);
    // float F = F(l, h);
    
    // float m1 = sqrt((nu+1)*(nv+1))/(8*3.14);
    // float m2 = 
}

float getLambertTerm(vec3 n, vec3 l)
{
    vec3 dir = vec3(1,0,0);
    float dotLT = dot(l, dir);
    vec3 projL = normalize(dir * dotLT + l);
    return max(0,dot(l, projL));
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
    
    lightSampleValues light0 = computePointLightValues(light0Pos, vec3(0,0,1), 40, pos);
    lightSampleValues light1 = computePointLightValues(light1Pos, vec3(0,0,1), 40, pos);
    lightSampleValues light2 = computePointLightValues(light2Pos, vec3(0,0,1), 120, pos);
    
    vec3 n = normalize(inNormal);    
    vec3 v = normalize(cameraPos - pos);

    vec3 cl0 = f(light0.L, v, n)*getLambertTerm(n, light0.L) * light0.iL;
    vec3 cl1 = f(light1.L, v, n)*getLambertTerm(n, light1.L) * light1.iL;
    vec3 cl2 = f(light2.L, v, n)*getLambertTerm(n, light2.L) * light2.iL;
    
    outColor = cl0 + cl1 + cl2;
}