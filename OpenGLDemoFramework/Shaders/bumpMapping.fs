#version 330 core

uniform sampler2D colorMap;
uniform sampler2D sampler;
uniform samplerCube cubeMap;

uniform float time;
uniform vec3 diffuse;
uniform vec3 cameraPos;
uniform float ior;
uniform mat4 mv;

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

vec3 computeBumpNormal(sampler2D sampler, vec2 uvs)
{
	float N  = texture2D(sampler, uvs + vec2(0, 0.001)).x;
	float NE  = texture2D(sampler, uvs + vec2(0.001, 0.001)).x;
	float E  = texture2D(sampler, uvs + vec2(0.001, 0)).x;
	float SE  = texture2D(sampler, uvs + vec2(0.001, -0.001)).x;
	float S  = texture2D(sampler, uvs + vec2(0, -0.001)).x;
	float SW  = texture2D(sampler, uvs + vec2(-0.001, -0.001)).x;
	float W  = texture2D(sampler, uvs + vec2(-0.001, 0)).x;
	float NW  = texture2D(sampler, uvs + vec2(-0.001, 0.001)).x;
	float C  = texture2D(sampler, uvs).x;
	
	float dx = ((NE + E + SE) - (NW + W + SW)) / 3.0;
	float dy = ((NW + N + NE) - (SW + S + SE)) / 3.0;
	
	return vec3(dx, 0, dy) * 4.0f;
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
    
    lightSampleValues light = computeDirLightValues(normalize(vec3(1, 0.0, 0.5)), 0.5);
    lightSampleValues light2 = computeDirLightValues(normalize(vec3(-1, 0.0, 0.5)), 0.5);
    //lightSampleValues light = computePointLightValues(vec4(0, 3, 8, 0).xyz, vec3(0,0,1), 4, pos);
    
    vec3 n = normalize((mv * vec4(computeBumpNormal(colorMap, inUVs),0.0)).xyz + normalize(inNormal));
    
    vec4 scw = shadowCoord / shadowCoord.w;
    scw.z -= 0.0005;
    float dfl = texture2D(sampler, scw.st).z;
    float sh = 1.0;
    if (shadowCoord.w > 0.0)
    {
        sh = dfl < scw.z ? 0.5 : 1.0;
    }
    
    //outColor = inColor;
    // outColor = sh * (lambert * vec3(1.0) + (texture2D(colorMap, 
                              // inUVs + vec2(-time * 0.005, (sin(time * 0.01)) * 0.5)).x ) * 
                              // (vec3(1,1,1) - diffuse));
    //outColor = sh * texture2D(colorMap, inUVs);
	
	vec3 r = normalize(reflect(n, light.L));
    vec3 r2 = normalize(reflect(n, light2.L));
    vec3 diff = diffuseColor;
    vec3 v = normalize(cameraPos - pos);
    //outColor = normalize(n);
    //outColor = (vec3(1,1,0) + texture(cubeMap, r).rgb) * lambert * 0.33 + vec3(1,1,1) * pow(max(0.0, dot(r, v)), 5.0) * 0.33;
        
    vec3 ambComp = computeAmbientComponent(light, ambColor, lightAmbDiffSpec, lightColor);
	vec3 diffComp = computeDiffuseComponent(n, light, diff, lightAmbDiffSpec, lightColor);
	vec3 specComp = computeSpecularComponent(n, pos, light, specularColor, specExp, lightAmbDiffSpec, texture(cubeMap, r).xyz, cameraPos);
    
    vec3 diffComp2 = computeDiffuseComponent(n, light2, diff, lightAmbDiffSpec, lightColor);
	vec3 specComp2 = computeSpecularComponent(n, pos, light2, specularColor, specExp, lightAmbDiffSpec, texture(cubeMap, r2).xyz, cameraPos);
    
    outColor = linearToSRGB(ambComp + diffComp + specComp + diffComp2 + specComp2); 	
	// outColor = ambComp + diffComp + specComp + diffComp2 + specComp2; 
	//outColor = texture(colorMap, inUVs).xyz;
}