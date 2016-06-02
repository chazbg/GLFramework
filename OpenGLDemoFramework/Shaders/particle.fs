#version 330 core

smooth in vec2 texCoords;
flat in float alpha;

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D sampler;

void main()
{
    color.xyz = texture(sampler, texCoords).rgb;
    color.a = color.x * alpha;
}