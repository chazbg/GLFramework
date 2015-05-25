#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler;

vec2 multComplex(vec2 a, vec2 b)
{   
    vec2 res;
    res.x = a.x * b.x - a.y * b.y;
    res.y = a.x * b.y + a.y * b.x;
    return res;
}

vec2 powComplex(vec2 a, int power)
{
    vec2 res = a;
    
    for (int i = 1; i < power; i++)
    {
        res = multComplex(res, a);
    }
    
    return res;
}

void main()
{
    float scale = 2.0 ;
    int iter = int((abs(sin(float(time) * 0.01))) * 20.0);
    int i = 0;
    
    vec2 c;
    c.x = (texCoords.x - 0.6666) * 3.0;
    c.y = 1.3333 * (texCoords.y - 0.5) * scale;
    vec2 z = vec2(0,0);
    for (i = 0; i < iter; i++)
    {
        vec2 zn = powComplex(z, 2) + c;
        
        if (length(zn) > 2.0) { break; }
        
        z = zn;
    }
    
    if (i == iter)
    {
        color = texture2D(sampler, vec2(0,0)).rgb;
    }
    else
    {
        color = texture2D(sampler, vec2(float(i)/100.0, 1)).rgb;
    }
}