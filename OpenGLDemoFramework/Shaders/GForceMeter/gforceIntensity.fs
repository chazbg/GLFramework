#version 330 core
in vec2 interpolatedCoords;
in vec2 texCoords;

uniform vec2 p;
uniform vec2 v;
uniform sampler2D prevIntensity;

// Ouput data
layout(location = 0) out vec3 intensity;

#define R 0.03
#define STEPS 10.0

void main()
{
    vec2 pos = p - v;
    float delta = length(v) / STEPS;
    
    intensity = texture2D(prevIntensity, texCoords).rgb * 0.98;
    for (float i = 0.0; i < STEPS; i += 1.0)
    {
        float dist = length(interpolatedCoords - p);
        if (dist <= R)
        {
            intensity = vec3(1.0);
        }
        pos += delta * v;
    }

	//intensity = vec3(p, length(v));
    //intensity = vec3(0.0, 0.5, 0.0);
}