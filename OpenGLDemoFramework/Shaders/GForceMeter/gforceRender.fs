#version 330 core
in vec2 interpolatedCoords;
in vec2 texCoords;

uniform sampler2D intensity;

// Ouput data
layout(location = 0) out vec3 color;

void main()
{
	color = texture2D(intensity, texCoords).rgb;
}