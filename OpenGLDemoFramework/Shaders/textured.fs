#version 330 core

uniform sampler2D tex;
uniform mat4 modelToWorld;

// Input data
smooth in vec3 vNormal;
smooth in vec2 vUV;

// Ouput data
out vec3 outColor;

void main()
{
    vec3 l    = normalize(modelToWorld * normalize(vec4(0.1, 0.1, 0.1, 0.0))).xyz;
    float NoL = dot(normalize(vNormal), l);
    outColor  = NoL * texture2D(tex, vec2(vUV.x, 1.0 - vUV.y)).bgr;
}