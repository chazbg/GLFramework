#version 330 core
in vec3 interpolatedColor;

// Ouput data
out vec3 color;

void main()
{
	color = interpolatedColor;
}