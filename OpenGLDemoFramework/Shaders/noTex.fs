#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler;

// float f(float x, float z)
// {
    // return sin(x)*sin(z);
// }

float f(float x, float z)
{
    return texture2D(sampler, vec2(x,z)).r;
}

// float f(float x, float z)
// {
    // float radius = 0.3f;
    // float xzLength = sqrt(x * x + z * z);
    
    // float y = 0;
    // if (xzLength * xzLength  <= radius * radius)
    // {
        // y = sqrt(radius * radius - xzLength * xzLength);
    // }

    // return y;
// }

bool castRay(const vec3 ro, const vec3 rd, out float resT)
{
    const float delt = 0.01f;
    const float mint = 1.0f;
    const float maxt = 10.0f;
    for(float t = mint; t < maxt; t += delt)
    {
        vec3 p = ro + rd*t;
        if(p.y < f(p.x, p.z))
        {
            resT = t - 0.5f*delt;
            return true;
        }
    }
    return false;
}
    
void genRay(float x, float y, out vec3 origin, out vec3 dir)
{
    // origin = vec3(sin(float(time) * 0.02),0.5, sin(float(time)*0.01));
    // vec3 end = vec3(x,y,1.0);
    origin = vec3(sin(time * 0.01f) * 2.0f, 2.0f, sin(time * 0.01f) * 2.0f);
    vec3 end = vec3(x,0,y);
    dir = end - origin;
}

vec3 getNormal(vec3 p)
{
    float eps = 0.01;
    vec3 n = vec3(f(p.x-eps,p.z) - f(p.x+eps,p.z),
                 2.0f*eps,
                 f(p.x,p.z-eps) - f(p.x,p.z+eps));
    return normalize(n);
}

vec3 terrainColor(vec3 origin, vec3 direction, float t)
{
    vec3 p = origin + direction * t;
    vec3 n = getNormal(p);
    return  abs(dot(n,direction)) * vec3(0.4,0.2,0);
}

void main()
{
    float res = 0;
    vec3 origin, dir;
    genRay(interpolatedCoords.x,interpolatedCoords.y,origin,dir);
    if (castRay(origin, dir, res))
    {
        color = terrainColor(origin, dir, res);
    }
    else
    {
        color = vec3(0,0,0);
    }
}