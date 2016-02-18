#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;

uniform uint time;
uniform mat4 mvp;

// uniform vec3 materialDiffuseColor;
// uniform vec3 materialAmbientColor;
// uniform vec3 materialSpecularColor;
// uniform float materialSpecularExp;
// uniform vec3 lightAmbDiffSpec;
// uniform vec3 lightColor;

smooth out vec3 inColor;

struct lightSampleValues {
	vec3 L;
	float iL;
};

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

vec3 computeSpecularComponent(vec3 surfaceNormal, vec4 surfacePosition, lightSampleValues light, vec3 materialSpecularColor, float materialSpecularExp, vec3 lightAmbDiffSpec, vec3 lightColor)
{
	vec3 viewVector = normalize(vec3(0,0,1)-surfacePosition.xyz);
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

lightSampleValues computePointLightValues(vec3 pointLightPosition, vec3 pointLightAttenuation, float pointLightIntensity, vec4 surfacePosition)
{
	lightSampleValues values;
	values.L = pointLightPosition.xyz - surfacePosition.xyz;
	float dist = length(values.L);
	values.L = values.L / dist; // normalize
	// Dot computes the 3-term attenuation in one operation
	// k_c * 1.0 + k_l * dist + k_q * dist * dist
	float distAtten = dot(pointLightAttenuation, vec3(1.0, dist, dist*dist));
	values.iL = pointLightIntensity / distAtten;
	return values;
}

lightSampleValues computeSpotLightValues(vec3 spotLightPosition, 
	vec3 spotLightAttenuation, 
	vec3 spotLightDir, 
	float spotLightAngleCos, 
	float spotLightExponent,
	float spotLightIntensity,
	vec4 surfacePosition)
{
	lightSampleValues values;
	values.L = spotLightPosition.xyz - surfacePosition.xyz;
	float dist = length(values.L);
	values.L = values.L / dist; // normalize
	// Dot computes the 3-term attenuation in one operation
	// k_c * 1.0 + k_l * dist + k_q * dist * dist
	float distAtten = dot(spotLightAttenuation,	vec3(1.0, dist, dist*dist));
	float spotAtten = dot(-spotLightDir, values.L);
	spotAtten = (spotAtten > spotLightAngleCos) ? pow(spotAtten, spotLightExponent) : 0.0;
	values.iL = spotLightIntensity * spotAtten / distAtten;
	return values;
}

void main(){
	vec3 ambColor = vec3(0.1, 1, 0.1);
	vec3 emissiveColor = vec3(1,1,1);
	vec4 diffuseColor = vec4(1,0,0,1);
	vec3 specularColor = vec3(1,1,1);
	vec3 lightAmbDiffSpec = vec3(0.2,1,1);
	vec3 lightColor = vec3(1,1,1);
	float specExp = 3.0;
	float angle = float(time) * 0.02;
	mat4 rot;
    rot[0] = vec4(cos(angle), 0, sin(angle), 0);
    rot[1] = vec4(0, 1, 0, 0);
    rot[2] = vec4(-sin(angle), 0, cos(angle), 0);
    rot[3] = vec4(0, 0, 0, 1);
	
	lightSampleValues light = computePointLightValues(vec3(3, 3, 5), vec3(0,0,1), 1, vec4(vertexPosition_modelspace, 1));
    gl_Position.xyz = vertexPosition_modelspace.xyz;
    gl_Position.w = 1.0;
	vec3 ambComp = computeAmbientComponent(light, ambColor, lightAmbDiffSpec, lightColor);
	vec3 diffComp = computeDiffuseComponent(normal, light, diffuseColor.xyz, lightAmbDiffSpec, lightColor);
	vec3 specComp = computeSpecularComponent(normal, gl_Position, light, specularColor, specExp, lightAmbDiffSpec, lightColor);
    gl_Position = mvp * gl_Position;
    inColor.xyz = ambComp + diffComp;
}

