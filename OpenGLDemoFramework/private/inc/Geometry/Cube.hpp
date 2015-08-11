#pragma once

#include "Core/Mesh.hpp"

class Cube : public Mesh
{
public:
	Cube();
	~Cube();

	void SetTime(GLuint time);
	GLuint GetTime();

	virtual void Render();
	virtual void RenderToTexture(const unsigned int texId);
	virtual void SetShaders(const string vertexShaderPath, const string fragmentShaderPath);
private:
	float* genVerts();
	GLuint vertexBufferID;
	GLuint normalsBufferID;
	GLuint programID;
	GLuint timeID;
	GLuint time;
};
