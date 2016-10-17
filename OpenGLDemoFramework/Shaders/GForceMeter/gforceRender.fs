#version 330 core
in vec2 interpolatedCoords;
in vec2 texCoords;

uniform sampler2D intensity;
uniform sampler2D gradient;

// Ouput data
layout(location = 0) out vec3 color;

vec3 getColor(float i)
{
    return texture2D(gradient, vec2(i, 0.0)).bgr;
}

void main()
{
	color = getColor(texture2D(intensity, texCoords).r);
}