#version 330 core

uniform vec3 diffuse;
uniform float alpha;

smooth in vec3 vNormal;
smooth in vec3 lightDir;

// Ouput data
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(vec3(0.1) + diffuse * max(0, dot(vNormal, lightDir)), alpha);
}