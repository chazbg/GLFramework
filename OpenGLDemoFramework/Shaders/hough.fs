#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D sampler;

void main()
{
    if (interpolatedCoords.x > -0.55 && interpolatedCoords.x < -0.45)
    {
        color = vec3(0,0,0);
    }
    else
    {
        color = vec3(1,1,0);
    }
    
}