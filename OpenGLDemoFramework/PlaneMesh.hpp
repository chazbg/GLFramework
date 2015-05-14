#pragma once
#include "Mesh.hpp"
class PlaneMesh : public Mesh
{
public:
	PlaneMesh(int _width, int _height);
	~PlaneMesh();

	void SetTime(GLuint time);
	GLuint GetTime();

	void UseProgram();
	void Render();
private:
	GLuint vertexBufferID;
	GLuint normalsBufferID;
	GLuint programID;
	GLuint timeID;
	GLuint time;
	GLfloat* buffer;
private:
	int width;
	int height;
};

