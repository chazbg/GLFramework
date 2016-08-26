#version 330 core
uniform float time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D   sampler0;
uniform samplerCube sampler1;

const vec2 resolution = vec2(1024.0, 1024.0);

float radicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N) 
{
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}

// Transform UVs from [0..1] to [-1..1] space
vec2 normalizeUVs(vec2 uvs)
{
	return uvs * 2.0 - 1.0;
}

struct Sphere
{
	vec3 pos;
	float r;
    float ior;
};

#define SPHERE_COUNT 36
Sphere spheres[SPHERE_COUNT];

void initializeSpheres()
{    
    for (int sphereIt = 0; sphereIt < SPHERE_COUNT; sphereIt++)
    {
        float sqrtCount = sqrt(float(SPHERE_COUNT));
        
   		float i = (mod(float(sphereIt), sqrtCount)  / sqrtCount) * 2.0 - 1.0;
        float j = (float(sphereIt / int(sqrtCount)) / sqrtCount) * 2.0 - 1.0;
        
        spheres[sphereIt].pos = vec3(i, j, 0.0) * 400.0;
		spheres[sphereIt].r   = 64.0;
        spheres[sphereIt].ior = 1.0 / 1.46;
    }
}

vec3 getCubemapColor(vec3 viewDir, vec3 normal)
{
    vec3 R = reflect(viewDir, normal);
    return texture(sampler1, R).rgb;
}

bool getIntersection(vec3 ray, vec3 cameraPos, out vec3 intersection, out Sphere s, out vec3 n)
{
    bool res = false;
    float nearestIntersection = 1000000000.0;
    
    for (int i = 0; i < SPHERE_COUNT; i++)
	{
        vec3 cameraToCenter = spheres[i].pos - cameraPos;
        float projLength = dot(cameraToCenter, ray);
        if (projLength > 0.0)
        {
        
            vec3 rayProj = ray * projLength;
            float distanceToRay = length(rayProj - cameraToCenter);

            float r = spheres[i].r;
            if (distanceToRay < r)
            {
                float distToIntersection = projLength - sqrt(r * r - distanceToRay * distanceToRay);

                if (distToIntersection < nearestIntersection)
                {
                    nearestIntersection = distToIntersection;
                    intersection = cameraPos + distToIntersection * ray;
                    n = normalize(intersection - spheres[i].pos);
                    s = spheres[i];
                    res = true;
                }
            }
        }
	}
    return res;
}

bool getIntersection2(vec3 ray, vec3 cameraPos, out vec3 intersection, Sphere s, out vec3 n)
{
    bool res = false;
  
    vec3 cameraToCenter = s.pos - cameraPos;
    float projLength = dot(cameraToCenter, ray);
    if (projLength > 0.0)
    {
        vec3 rayProj = ray * projLength;
        float distanceToRay = length(rayProj - cameraToCenter);

        float r = s.r;
        if (distanceToRay < r)
        {
            float distToIntersection = projLength - sqrt(r * r - distanceToRay * distanceToRay);
            intersection = cameraPos + distToIntersection * ray;
            n = normalize(intersection - s.pos);
            res = true;
        }
    }
    
    return res;
}

vec3 getCameraPos(float time)
{
    mat3 rot = mat3(
        vec3(cos(time), 0.0, -sin(time)),
        vec3(0.0,       1.0,  0.0),
        vec3(sin(time), 0.0,  cos(time)));
    return rot * vec3(0.0, 0.0, 1000.0);
}

vec3 getViewDirection(vec3 cameraPos)
{
    return normalize(vec3(0.0) - cameraPos);
}

float F(vec3 n, vec3 v, float ior)
{
    float r0 = ior * ior;
    return ior + (1.0 - ior) * pow(1.0 - dot(n, v), 5.0);
}

#define SAMPLES 128

void main()
{
	initializeSpheres();
	
	vec3  lightDir    = normalize(vec3(1.0, 1.0, 1.0));
	vec2  offset      = interpolatedCoords;
	vec3  cameraPos   = getCameraPos(time * 0.01);
	vec3  cameraView  = getViewDirection(cameraPos);
	vec3  cameraUp    = vec3(0.0, 1.0,  0.0);
	vec3  cameraRight = cross(cameraView, cameraUp);
	float cameraNear  = 4.0;
	
	vec3 ray = normalize(cameraRight * offset.x * (resolution.x / resolution.y) + cameraUp * -offset.y + cameraView * cameraNear);
	
	Sphere s;
    s.pos = vec3(0.0);
    s.r = 200.0;
    s.ior = 1.0 / 1.46;
	float roughness = abs(sin(time * 0.03));
    vec3 intr;
    vec3 n;
	color = vec3(0.0);
    if(getIntersection2(ray, cameraPos, intr, s, n))
    {
		vec3 up = vec3(0.0, 1.0, 0.0);
        if (1.0 - abs(dot(n, up)) < 0.001)
		{
			up = vec3(1.0, 0.0, 0.0);
		}
		
		vec3 rightVec = normalize(cross(n, up));
        vec3 upVec = normalize(cross(rightVec, n));
        
		vec2 offsets[SAMPLES];
		vec3 H[SAMPLES];
		
		vec3 refl = reflect(ray, n);
		
		for (int i = 0; i < SAMPLES; i++)
		{
			offsets[i] = Hammersley(uint(i), uint(SAMPLES)) * roughness;
			H[i] = normalize(refl + upVec * offsets[i].x + rightVec * offsets[i].y);
			color += texture(sampler1, H[i]).rgb;
		}
        //offset1.xy *= roughness;
		color /= float(SAMPLES);
    }
    else
    {
        color = texture(sampler1, ray).rgb;
    }
    
}
