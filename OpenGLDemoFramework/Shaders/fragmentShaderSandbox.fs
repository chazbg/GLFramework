#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
layout(location = 0) out vec3 color;

uniform sampler2D sampler;

void main()
{
	color = texture2D(sampler, texCoords).rgb;
    //color.g = abs(sin(float(time)*0.01)) / 2.0;
}