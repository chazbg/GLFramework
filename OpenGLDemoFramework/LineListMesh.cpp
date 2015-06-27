#include "LineListMesh.hpp"
#include "Shader.hpp"
#include <cmath>

#define BUFFER_OFFSET(i) ((void*)(i))

LineListMesh::LineListMesh(const std::vector<Vec2>& pointsList, const Vec3& color, const float lineSize) :
Mesh(), color(color), lineSize(lineSize)
{
	vertexCount = pointsList.size() * 2;
	vertexBuffer = genVerts(pointsList);

	programID = LoadShaders("Shaders/primitives2d.vs", "Shaders/primitives2d.fs");
	timeID = glGetUniformLocation(programID, "time");
	colorID = glGetUniformLocation(programID, "color");

	time = 0;

	UseProgram();

	glUniform1ui(timeID, time);
	glUniform3f(colorID, color.x, color.y, color.z);

	glGenBuffers(1, &vertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * 4, vertexBuffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		BUFFER_OFFSET(0)
		);

	glDisableVertexAttribArray(0);
}

LineListMesh::~LineListMesh()
{
}

void LineListMesh::SetTime(const GLuint time)
{
	this->time = time;
	glUniform1ui(timeID, time);
}

GLuint LineListMesh::GetTime() const
{
	return time;
}

void LineListMesh::SetColor(const Vec3& color)
{
	this->color = color;
	glUniform3f(colorID, color.x, color.y, color.z);
}

Vec3 LineListMesh::GetColor() const
{
	return color;
}

void LineListMesh::SetLineSize(const float lineSize)
{
	this->lineSize = lineSize;
}

float LineListMesh::GetLineSize() const
{
	return lineSize;
}

void LineListMesh::UseProgram()
{
	glUseProgram(programID);
}

void LineListMesh::Render()
{
	UseProgram();
	SetTime(time + 1);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		BUFFER_OFFSET(0)
		);

	glLineWidth(lineSize);
	glDrawArrays(GL_LINES, 0, vertexCount);

	glDisableVertexAttribArray(0);
}

float* LineListMesh::genVerts(const std::vector<Vec2>& pointsList)
{
	float* verts = new float[pointsList.size() * 4];

	for (unsigned int i = 0; i < pointsList.size(); i++)
	{
		verts[4 * i] = pointsList[i].x;
		verts[4 * i + 1] = pointsList[i].y;
		verts[4 * i + 2] = pointsList[(i + 1) % pointsList.size()].x;
		verts[4 * i + 3] = pointsList[(i + 1) % pointsList.size()].y;
	}

	return verts;
}