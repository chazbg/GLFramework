#version 330 core

// Input data
smooth in vec3 vNormal;
flat   in float vOpacity;

// Ouput data
out vec4 outColor;

void main()
{
    vec3 d   = vec3(0.2, 0.5, 0.1);
    vec3 l1  = normalize(vec3(0.0,  0.0,  1.0));
    vec3 n   = normalize(vNormal);
    
    float NoL1 = max(0.0, dot(n, l1));
    
    outColor.rgb = d * pow(NoL1, 4.0);
    outColor.a   = vOpacity;
}