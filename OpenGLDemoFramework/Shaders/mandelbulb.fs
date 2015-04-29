#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler;

vec3 rotate(vec3 pos, float x, float y, float z)
{
	mat3 rotX = mat3(1.0, 0.0, 0.0, 0.0, cos(x), -sin(x), 0.0, sin(x), cos(x));
	mat3 rotY = mat3(cos(y), 0.0, sin(y), 0.0, 1.0, 0.0, -sin(y), 0.0, cos(y));
	mat3 rotZ = mat3(cos(z), -sin(z), 0.0, sin(z), cos(z), 0.0, 0.0, 0.0, 1.0);

	return rotX * rotY * rotZ * pos;
}

float f(vec3 r)
{
    r = rotate(r, sin(float(time) * 0.01), cos(float(time) * 0.01), 0.0);
    vec3 zn = vec3(r.xyz);
    float rad = 0.0;
    float hit = 0.0;
    float p = 8.0;
    float d = 1.0;
    int iter = int(float(mod(time, 200) * 0.05));
    for(int i = 0; i < iter; i++)
    {
        rad = length(zn);

        if(rad > 2.0)
        {	
            hit = 0.5 * log(rad) * rad / d;
            break;
        }else{
            float th = atan(length(zn.xy), zn.z);
            float phi = atan(zn.y, zn.x);		
            float rado = pow(rad,8.0);
            d = pow(rad, 7.0) * 7.0 * d + 1.0;
            
            float sint = sin(th * p);
            zn.x = rado * sint * cos(phi * p);
            zn.y = rado * sint * sin(phi * p);
            zn.z = rado * cos(th * p) ;
            zn += r;
        } 
    }
	
	return hit;
}

float castRay(const vec3 ro, const vec3 rd)
{
    float t = 0.0;
	float d = 200.0;
	
	vec3 r;

	for(int i = 0; i < 100; i++){	
		if(d > .00001)
		{	
			r = ro + rd * t;
			d = f(r);
			t+=d;
		}
	}
    
    return t;
}
    
void genRay(float x, float y, out vec3 origin, out vec3 dir)
{
	origin = vec3(x, y, -1.2 );
	vec3 la = vec3( 0.0, 0.0, 1.0 );
	
	vec3 cameraDir = normalize(la - origin);
	vec3 cameraRight = normalize(cross(cameraDir, vec3( 0.0, 1.0, 0.0 )));
	vec3 cameraUp = normalize(cross(cameraRight, cameraDir));

	dir = normalize(cameraDir + vec3(x, y, 0.0));
}

vec3 getNormal(vec3 r)
{
    vec3 eps = vec3(0.01f, 0.0f, 0.0f);
    vec3 n = vec3(f(r + eps) - f(r - eps),
	  			  f(r + eps.yxz) - f(r - eps.yxz),
	  			  f(r + eps.zyx) - f(r - eps.zyx));
    return normalize(n);
}

vec3 terrainColor(vec3 origin, vec3 direction, float t)
{
    vec3 p = origin + direction * t;
    vec3 n = getNormal(p);
    return  abs(dot(n,direction)) * texture2D(sampler, texCoords).rgb;
}

void main()
{
    vec2 c;
    c.x = (texCoords.x - 0.6666) * 3.0;
    c.y = 1.3333 * (texCoords.y - 0.5) * 2.0;
    float res = 0;
    vec3 origin, dir;
    genRay(c.x,c.y,origin,dir);
    res = castRay(origin, dir);
    color = terrainColor(origin, dir, res);
}