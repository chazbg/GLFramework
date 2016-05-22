#version 330 core

uniform vec3 diffuse;
uniform float alpha;
uniform sampler2D sampler;

smooth in vec3 vNormal;
smooth in vec3 lightDir;
smooth in vec2 vUVs;

// Ouput data
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(0.2 * diffuse + 0.8 * texture(sampler, vUVs).xyz * max(0, dot(vNormal, lightDir)), alpha);
}