#version 330 core

// Input data
smooth in vec3 vNormal;

// Ouput data
out vec3 outColor;

void main()
{
    vec3 d   = vec3(0.2, 0.5, 0.1);
    vec3 l1  = normalize(vec3(0.0,  1.0, -1.0));
    vec3 l2  = normalize(vec3(1.0,  1.0,  1.0));
    vec3 l3  = normalize(vec3(-1.0, 1.0,  1.0));
    vec3 n   = normalize(vNormal);
    
    float NoL1 = max(0.0, dot(n, l1));
    float NoL2 = max(0.0, dot(n, l2));
    float NoL3 = max(0.0, dot(n, l3));
    
    outColor = d * (NoL1 + NoL2 + NoL3) / 3.0;
}