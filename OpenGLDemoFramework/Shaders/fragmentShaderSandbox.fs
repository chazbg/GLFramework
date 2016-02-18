#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
layout(location = 0) out vec3 color;

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
void main()
{
	color = (texture2D(sampler1, texCoords).rgb + texture2D(sampler2, texCoords).rgb) / 2.0;
}