#version 330 core
in vec2 texCoords;

// Ouput data
out vec3 color;

uniform sampler2D videoFrame;

void main()
{
	color = texture2D(videoFrame, texCoords).rgb;
    //color = vec3(texCoords,0);
}