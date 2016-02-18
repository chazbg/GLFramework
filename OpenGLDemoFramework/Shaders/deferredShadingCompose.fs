#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
layout(location = 0) out vec3 color;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

void main()
{
    vec3 light = normalize(vec3(0, 0.15, 0.25));
    vec3 n = texture2D(normalMap, texCoords).rgb;
    vec3 c = texture2D(colorMap, texCoords).rgb;
	color = dot(n.xyz, light) * c;
}