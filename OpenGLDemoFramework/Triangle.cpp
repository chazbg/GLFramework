#include "Triangle.hpp"
#include "Shader.hpp"
#include <cmath>

Triangle::Triangle() :
Mesh()
{
	vertexCount = 3;
	vertexBuffer = genVertices();
	generateNormals();
	programID = LoadShaders("Shaders/basic.vs", "Shaders/basic.fs");
	timeID = glGetUniformLocation(programID, "time");
	startAngleID = glGetUniformLocation(programID, "startAngle");
	offsetAngleID = glGetUniformLocation(programID, "offsetAngle");

	time = 0;
	startAngle = 0;
	offsetAngle = 0;

	glUniform1ui(timeID, time);
	glUniform1f(startAngleID, startAngle);
	glUniform1f(offsetAngleID, offsetAngle);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, vertexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,		
		3,                 
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, normalsBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

Triangle::~Triangle()
{
	
}

void Triangle::SetTime(GLuint time)
{
	this->time = time;
	glUniform1ui(timeID, time);
}

GLuint Triangle::GetTime()
{
	return time;
}

void Triangle::SetStartAngle(GLfloat startAngle)
{
	this->startAngle = startAngle;
	glUniform1f(startAngleID, startAngle);
}

GLfloat Triangle::GetStartAngle()
{
	return startAngle;
}

void Triangle::SetOffsetAngle(GLfloat offsetAngle)
{
	this->offsetAngle = offsetAngle;
	glUniform1f(offsetAngleID, offsetAngle);
}

GLfloat Triangle::GetOffsetAngle()
{
	return offsetAngle;
}

void Triangle::UseProgram()
{
	glUseProgram(programID);
}

void Triangle::Render()
{
	UseProgram();
	SetTime(time + 1);
	SetOffsetAngle(3.14f / 2.0f);
	SetStartAngle(3.0f * 3.14f / 2.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

float* Triangle::genVertices()
{
	float* verts = new float[9];

	verts[0] = -0.5f;
	verts[1] = -0.5f;
	verts[2] = 0.0f;
	verts[3] = 0.5f;
	verts[4] = -0.5f;
	verts[5] = 0.0f;
	verts[6] = 0.0f;
	verts[7] = 0.5f;
	verts[8] = 0.0f;

	return verts;
}