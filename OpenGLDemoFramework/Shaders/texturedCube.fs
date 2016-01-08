#version 330 core

uniform sampler2D colorMap;
uniform sampler2D sampler;
uniform samplerCube cubeMap;

uniform float time;
uniform vec3 diffuse;
uniform vec3 cameraPos;

// Input data
smooth in vec3 inNormal;
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;
smooth in vec3 pos;

// Ouput data
layout(location = 0) out vec3 outColor;

void main()
{
	vec3 light = normalize(vec3(0, 0.15, 0.25));
    vec3 n = normalize(inNormal);
	float lambert = dot(n, light);
    vec4 scw = shadowCoord / shadowCoord.w;
    scw.z -= 0.0005;
    float dfl = texture2D(sampler, scw.st).z;
    float sh = 1.0;
    if (shadowCoord.w > 0.0)
    {
        sh = dfl < scw.z ? 0.5 : 1.0;
    }
    
    //outColor = inColor;
    // outColor = sh * (lambert * vec3(1.0) + (texture2D(colorMap, 
                              // inUVs + vec2(-time * 0.005, (sin(time * 0.01)) * 0.5)).x ) * 
                              // (vec3(1,1,1) - diffuse));
    //outColor = sh * texture2D(colorMap, inUVs);
	
	vec3 r = normalize(reflect(n, light));
	//outColor = texture(cubeMap, r);
    vec3 v = normalize(cameraPos - pos);
    //outColor = normalize(n);
    outColor = (vec3(1,1,0) + texture(cubeMap, r).rgb) * lambert * 0.33 + vec3(1,1,1) * pow(max(0.0, dot(r, v)), 5.0) * 0.33;
}