#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D sampler;
uniform float alpha;

void main()
{
	color.xyz = texture(sampler, texCoords).rgb;
    color.a = color.x * alpha;
}