#include "PlaneMesh.hpp"
#include "PlaneGenerator.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

PlaneMesh::PlaneMesh(int _width, int _height) : 
width(_width), 
height(_height), 
vertexBuffer(generatePlaneVertices(width, height))
{
	programID = LoadShaders("Shaders/plane.vs", "Shaders/plane.fs");
	timeID = glGetUniformLocation(programID, "time");

	time = 0;

	glUniform1ui(timeID, time);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	GenerateNormals();
	glBufferData(GL_ARRAY_BUFFER, width * height * 18 * 4, vertexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		BUFFER_OFFSET(0)
		);

	UseProgram();
}


PlaneMesh::~PlaneMesh()
{
	delete[] vertexBuffer;
}

void PlaneMesh::SetTime(GLuint time)
{
	this->time = time;
	glUniform1ui(timeID, time);
}

GLuint PlaneMesh::GetTime()
{
	return time;
}

void PlaneMesh::UseProgram()
{
	glUseProgram(programID);
}

void PlaneMesh::Render()
{
	UseProgram();
	SetTime(time + 1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, width * height * 18); // 12 + 12 + 12 indices starting at 0 -> 4 + 4 + 4 triangles

	glDisableVertexAttribArray(0);
}

void PlaneMesh::GenerateNormals()
{
}