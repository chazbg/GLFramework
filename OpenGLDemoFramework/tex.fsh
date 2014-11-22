#version 330 core
in vec2 texCoords;

// Ouput data
out vec3 color;

void main()
{

	// Output color = red 
	color = vec3(texCoords,0);

}