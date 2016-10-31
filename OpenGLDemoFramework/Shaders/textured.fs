#version 330 core

uniform sampler2D tex;

// Input data
smooth in vec3 vNormal;
smooth in vec2 vUV;

// Ouput data
out vec3 outColor;

void main()
{
    vec3 l    = normalize(vec3(0.1));
    float NoL = dot(normalize(vNormal), l);
    outColor  = NoL * texture2D(tex, vec2(vUV.x, 1.0 - vUV.y)).bgr;
}