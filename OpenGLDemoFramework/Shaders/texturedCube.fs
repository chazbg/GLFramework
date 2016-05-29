#version 330 core

uniform sampler2D colorMap;
uniform sampler2D sampler;

uniform float time;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 cameraPos;

// Input data
smooth in vec3 inNormal;
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;
smooth in vec3 pos;

// Ouput data
layout(location = 0) out vec3 outColor;

void main()
{
	vec3 ambColor = vec3(1, 1, 1);
	vec3 emissiveColor = vec3(1,1,1);
	vec3 diffuseColor = vec3(1,1,0);
	vec3 specularColor = vec3(1,1,1);
	vec3 lightColor = vec3(1,1,1);
	float specExp = 6.0;
    
    vec3 light = normalize(vec3(0, 0.15, 0.25));
    vec3 n = normalize(inNormal);
    
    float NoL = max(0.0, dot(n, light));
    
    vec3 texColor = texture(colorMap, inUVs + vec2(time * 0.01, sin(time * 0.01))).xyz;
    outColor = diffuse * texColor * NoL;
}