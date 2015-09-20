#version 330 core

uniform sampler2D sampler;

// Input data
smooth in vec3 inColor;
smooth in vec4 shadowCoord;

// Ouput data
out vec3 outColor;

void main()
{
    vec4 scw = shadowCoord / shadowCoord.w;
    scw.z += 0.0005;
    float dfl = texture2D(sampler, scw.st).z;
    float sh = 1.0;
    if (shadowCoord.w > 0.0)
    {
        sh = dfl < scw.z ? 0.5 : 1.0;
    }
    
    //outColor = inColor;
    outColor = sh * inColor;
}