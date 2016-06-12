#version 330 core
uniform float time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform vec3 cameraPos;

// float noise(vec3 p) //Thx to Las^Mercury
// {
	// vec3 i = floor(p);
	// vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
	// vec3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
	// a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
	// a.xy = mix(a.xz, a.yw, f.y);
	// return mix(a.x, a.y, f.z);
// }

// float f(float x, float z)
// {
    // return noise(vec3(x, z, float(time) * 0.003));
// }

bool f(vec3 p)
{
    float radius = 1.0f;
    float len = length(p);

    if (len  <= radius)
    {
        return true;
    }

    return false;
}

bool castRay(const vec3 ro, const vec3 rd, out float resT)
{
    const float delt = 0.01f;
    const float mint = 1.0f;
    const float maxt = 10.0f;
    for(float t = mint; t < maxt; t += delt)
    {
        vec3 p = ro + rd*t;
        if(f(p))
        {
            resT = t - 0.5f*delt;
            return true;
        }
    }
    return false;
}
    
void genRay(float x, float y, out vec3 origin, out vec3 dir)
{
    origin = cameraPos;
    //origin = vec3(0, 5.0f, 15.0f);
    vec3 end = vec3(0,0,0);
    vec3 zA = normalize(end - origin);
    vec3 xA = normalize(cross(zA, vec3(0,1,0)));
    vec3 yA = normalize(cross(xA, zA));
    
    dir = zA + xA * x + yA * y;
}

// vec3 getNormal(vec3 p)
// {
    // float eps = 0.01;
    // vec3 n = vec3(f(p.x-eps,p.z) - f(p.x+eps,p.z),
                 // 2.0f*eps,
                 // f(p.x,p.z-eps) - f(p.x,p.z+eps));
    // return normalize(n);
// }

vec3 getNormal(vec3 p)
{
    return normalize(p);
}

vec4 permute( vec4 x )
{
    return mod(((x*34.0)+1.0)*x, 289.0);
}

vec4 taylorInvSqrt( vec4 r )
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{
    const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );

    //  x0 = x0 - 0. + 0.0 * C
    vec3 x1 = x0 - i1 + 1.0 * C.xxx;
    vec3 x2 = x0 - i2 + 2.0 * C.xxx;
    vec3 x3 = x0 - 1. + 3.0 * C.xxx;

    // Permutations
    i = mod(i, 289.0 );
    vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

    // Gradients
    // ( N*N points uniformly over a square, mapped onto an octahedron.)
    float n_ = 1.0/7.0; // N=7
    vec3  ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );

    //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
    //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);

    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                  dot(p2,x2), dot(p3,x3) ) );
}

vec3 terrainColor(vec3 origin, vec3 direction, float t)
{
    vec3 p = origin + direction * t;
    vec3 n = getNormal(p);
    float c = abs( snoise( vec3(p.xz, time * 0.01) ) );
    c += 0.5 * abs( snoise( vec3(p.xz, time * 0.01) * 2.0 ) );
    c += 0.25 * abs( snoise( vec3(p.xz, time * 0.01) * 4.0 ) );
    c += 0.125 * abs( snoise( vec3(p.xz, time * 0.01) * 8.0 ) );
    vec3 col = vec3( 1.5 - c, 1.0 - c, 0.5 - c);
    return  abs(dot(n,direction)) * vec3(0.4,0.2,0) * col;
    // return vec3(1,0,1);
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
        color = vec3(0.7);
    }
}