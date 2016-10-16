#version 330 core
in vec2 interpolatedCoords;
in vec2 texCoords;

uniform vec2 p;
uniform vec2 v;

// Ouput data
layout(location = 0) out vec3 intensity;

#define R 0.05

void main()
{
    intensity = vec3(0.0);
    if (length(interpolatedCoords - p) <= R)
    {
        intensity = vec3(1.0);
    }
	//intensity = vec3(p, length(v));
    //intensity = vec3(0.0, 0.5, 0.0);
}