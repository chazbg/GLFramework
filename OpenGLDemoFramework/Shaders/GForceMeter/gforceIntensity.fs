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

vec3 getColor(float dist, float r)
{
    vec3 res = vec3(0.0);
    res.x    = 1.0 - dist / r;
    return res;
}

void main()
{
    vec2 pos = p - v;
    float delta = length(v) / STEPS;
    
    intensity = texture2D(prevIntensity, texCoords).rgb * 0.95;
    float hits = 0.0;
    vec3 additional = vec3(0.0);
    
    for (float i = 0.0; i < STEPS; i += 1.0)
    {
        float dist = length(interpolatedCoords - pos);
        
        if (dist <= R)
        {
            additional += getColor(dist, R);
            hits       += 1.0;
        }
        pos += delta * v;
    }

    if (hits > 1.0)
    {
        additional /= hits;
    }        
    
    intensity += additional;
	//intensity = vec3(p, length(v));
    //intensity = vec3(0.0, 0.5, 0.0);
}