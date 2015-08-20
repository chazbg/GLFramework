#version 330 core

uniform sampler2D sampler;

// Input data
smooth in vec3 inColor;
smooth in vec3 shadowCoord;

// Ouput data
out vec3 outColor;

void main()
{
    // if (1.0 - inColor.x < 0.05 || 1.0 - inColor.z < 0.05)
    // {
        // outColor = vec3(0.5,0.0,0.0);
        // return;
    // }
    // if (1.0 - inColor.y < 0.05)
    // {
        // outColor = vec3(0.0,0.5,0.0);
        // return;
    // }
    // if (1.0 - inColor.z < 0.05)
    // {
        // outColor = vec3(0.0,0.0,0.5);
        // return;
    // }
    // else
    // {
        // outColor = vec3(1,1,1);
    // }
    
    float visibility = 1.0;
    if (texture2D(sampler, shadowCoord.xy).z < shadowCoord.z)
    {
        visibility = 0.5;
    }
    
    //outColor = vec3(1, 0, 0);
    //outColor = shadowCoord.xyz;
    //outColor = texture2D(sampler, shadowCoord.xy).xyz;
    outColor = visibility * inColor;
}