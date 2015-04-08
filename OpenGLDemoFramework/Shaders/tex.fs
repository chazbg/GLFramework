#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler;

// float f(float x, float z)
// {
    // return sin(x)*sin(x)*sin(z);
// }

// float f(float _x, float _z)
// {
    // return 0.5;
// }

// float f(float _x, float _z)
// {
    // return texture2D(sampler, vec2(_x, _z)).r / 2;
// }

float f(float _x, float _z)
{
    int iter = 20; //int(sin(float(time) * 0.02) * 20.0);
    int i = 0;
    float scale = 2.0;
    vec2 c;
    c.x = 1.0 * (_x / 4.0 - 0.5) * scale;
    c.y = (_z / 4.0 - 0.5) * scale;
    // c.x = _x;
    // c.y = _z;
    vec2 z = vec2(0,0);
    for (i = 0; i < iter; i++)
    {
        float x = z.x * z.x - z.y * z.y + c.x;
        float y = z.x * z.y + z.y * z.x + c.y;
        
        if (x*x + y*y > 2.0)
        {
            break;
        }
        z.x = x;
        z.y = y;
    }
    
    return i / 40.0;
}

bool castRay(const vec3 ro, const vec3 rd, out float resT)
{
    const float delt = 0.01f;
    const float mint = 1.0f;
    const float maxt = 2.0f;
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
    origin = vec3(sin(float(time) * 0.02), abs(sin(float(time) * 0.01)), sin(float(time)*0.01));
    vec3 end = vec3(x,y,2.0);
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
    //dot(n, direction) *
    return  abs(dot(n,direction)) * texture2D(sampler, texCoords).rgb;//vec3(0.4,0.2,0);
}

void main()
{
    //color = vec3(sin(interpolatedCoords.x*100), sin(interpolatedCoords.y*100), 0);
    // if (interpolatedcoords.y - sin(interpolatedcoords.x * 3.14) * sin(interpolatedcoords.y * 3.14) >= 0.0001)
    // {
        // color = vec3(interpolatedcoords.y + 0.5,0,0);
    // }
    // else
    // {
        // color = vec3(1,1,1);
    // }
    float res = 0;
    vec3 origin, dir;
    // genRay(interpolatedCoords.x,interpolatedCoords.y,origin,dir);
    genRay(texCoords.x,texCoords.y,origin,dir);
    if (castRay(origin, dir, res))
    {
        color = terrainColor(origin, dir, res);
    }
    else
    {
        color = vec3(0.2,0.2,0.2);
    }
	//color = texture2D(videoFrame, texCoords).rgb;
    //color = vec3(interpolatedCoords, 0.0);
    //color = vec3(1.0, 1.0, 0.0);
}