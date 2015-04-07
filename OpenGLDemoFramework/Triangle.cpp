#include "Triangle.hpp"
#include "Shader.hpp"
#include <cmath>

const GLfloat Triangle::g_vertex_buffer_data[] = {
	//front side
	//1
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,
};

Triangle::Triangle()
{
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

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,		
		3,                 
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);
}

Triangle::~Triangle()
{
	glDisableVertexAttribArray(0);
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
	SetTime(time + 1);
	SetOffsetAngle(3.14f / 2.0f);
	SetStartAngle(3.0f * 3.14f / 2.0f);
	UseProgram();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
}