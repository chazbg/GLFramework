#version 330 core

uniform sampler2D sampler;

// Input data
smooth in vec3 inColor;
smooth in vec4 shadowCoord;

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
    
    vec4 scw = shadowCoord / shadowCoord.w;
    scw.z += 0.0005;
    float dfl = texture2D(sampler, scw.st).z;
    float sh = 1.0;
    if (shadowCoord.w > 0.0)
    {
        sh = dfl < scw.z ? 0.5 : 1.0;
    }
    
    // float visibility = 1.0;
    // if (texture2D(sampler, shadowCoord.xy).z < shadowCoord.z)
    // {
        // visibility = 0.5;
    // }
    

    //outColor = vec3(1, 0, 0);
    //outColor = vec3(texture2D(sampler, shadowCoord.xy).z, texture2D(sampler, shadowCoord.xy).z, texture2D(sampler, shadowCoord.xy).z);
    //outColor = vec3(shadowCoord.z, shadowCoord.z, shadowCoord.z);
    //outColor = texture2D(sampler, shadowCoord.xy).xyz;
    outColor = sh * inColor;
}