#version 330 core
in vec2 interpolatedCoords;
in vec2 texCoords;

uniform vec2 p;
uniform vec2 v;
uniform sampler2D prevIntensity;

// Ouput data
layout(location = 0) out vec3 intensity;

#define R 0.08
#define STEPS 25.0
#define INV_STEPS 0.04

vec3 getColor(float dist, float r)
{
    vec3 res = vec3(0.0);
    res.x    = pow(1.0 - dist / r, 4.0);
    return res;
}

void main()
{
    vec2 pos = p - v;
    vec2 delta = v * INV_STEPS;
    
    intensity = texture2D(prevIntensity, texCoords).rgb * 0.98;
    float hits = 0.0;
    vec3 additional = vec3(0.0);
    
    for (float i = 0.0; i < STEPS; i += 1.0)
    {
        float dist = length(interpolatedCoords - pos);
        
        if (dist <= R)
        {
            if (length(delta) > 0.0)
            {
                additional += getColor(dist, R);
                hits       += 1.0;
            }
        }
        pos += delta;
    }

    if (hits >= 1.0)
    {
        if (length(delta) == 0.0)
        {
            intensity *= 1.0 / 0.98;
        }
        additional /= hits;   
    }        
    
    intensity += additional;
	//intensity = vec3(p, length(v));
    //intensity = vec3(0.0, 0.5, 0.0);
}