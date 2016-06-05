#version 330 core
uniform uint time;
uniform vec2 resolution;
uniform vec2 motionBlurDir;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
layout(location = 0) out vec3 color;

uniform sampler2D sampler;

void main()
{
    color = vec3(0.0);
    
    for (float i = -0.4; i <= 0.4; i += 0.1)
    {
        color += texture2D(sampler, texCoords + i * motionBlurDir).rgb;
    }
    
    color /= 8.0;
}