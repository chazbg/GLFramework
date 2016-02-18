#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform vec3 cameraPos;

// float f(float x, float z)
// {
    // return sin(x)*sin(z);
// }

float noise(vec3 p) //Thx to Las^Mercury
{
	vec3 i = floor(p);
	vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
	vec3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
	a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
	a.xy = mix(a.xz, a.yw, f.y);
	return mix(a.x, a.y, f.z);
}

float f(float x, float z)
{
    return noise(vec3(x, z, float(time) * 0.003));
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
    origin = cameraPos;//vec3(0, 5.0f, 15.0f);
    vec3 end = vec3(0,0,0);
    vec3 zA = normalize(end - origin);
    vec3 xA = cross(zA, vec3(0,1,0));
    vec3 yA = cross(xA, zA);
    
    dir = zA + xA * x + yA * y;
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
    genRay((interpolatedCoords.x - 0.5) * 2.0,(interpolatedCoords.y - 0.5) * 2.0,origin,dir);
    if (castRay(origin, dir, res))
    {
        color = terrainColor(origin, dir, res) + texture2D(sampler2, (origin + dir * res).xz).xyz;
    }
    else
    {
        color = vec3(0,0,0);
    }
}