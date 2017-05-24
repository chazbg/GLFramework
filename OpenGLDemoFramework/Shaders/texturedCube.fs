#version 330 core

uniform sampler2D colorMap;
uniform sampler2D sampler;

uniform float time;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 cameraPos;

// Input data
smooth in vec3 inNormal;
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;

// Ouput data
layout(location = 0) out vec3 outColor;

void main()
{
    vec3 light    = normalize(vec3(0, 0.15, 0.25));
    vec3 n        = normalize(inNormal);
    float NoL     = max(0.0, dot(n, light));
    vec3 texColor = texture(colorMap, inUVs + vec2(time * 0.01, sin(time * 0.01))).xyz;
    
    vec4 scw      = (shadowCoord / shadowCoord.w) * 0.5 + 0.5;
    scw.z        -= 0.0005;
    float dfl     = texture(sampler, scw.st).z;
    float sh      = 1.0;
    if (shadowCoord.w > 0.0)
    {
        sh = dfl < scw.z ? 0.5 : 1.0;
    }
    
    outColor = diffuse * texColor * NoL * sh;
}