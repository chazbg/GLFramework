#pragma once

#include "Mesh.hpp"

class Cube : public Mesh
{
public:
	Cube();
	~Cube();

	void SetTime(GLuint time);
	GLuint GetTime();

	void Render();
	void RenderToTexture(const unsigned int texId);
private:
	float* genVerts();
	GLuint vertexBufferID;
	GLuint normalsBufferID;
	GLuint programID;
	GLuint timeID;
	GLuint time;
};
