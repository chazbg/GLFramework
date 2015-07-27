#version 330 core
 
// Ouput data
layout(location = 0) out float fragmentdepth;
//out vec3 outColor;

void main()
{
    //outColor = vec3(1,0,0);
    // Not really needed, OpenGL does it anyway
    fragmentdepth = gl_FragCoord.z;
}