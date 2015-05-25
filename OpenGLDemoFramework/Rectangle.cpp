#include "Rectangle.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Rectangle::Rectangle() : topLeft(-1, 1), bottomRight(1, -1)
{
	vertexBuffer = genVertices();

	initGL();
}

Rectangle::Rectangle(Vec2 topLeft, Vec2 bottomRight) : topLeft(topLeft), bottomRight(bottomRight)
{
	vertexBuffer = genVertices();

	initGL();
}

Rectangle::~Rectangle() {}

void Rectangle::Render()
{
	UseProgram();

	SetTime(time + 1);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Rectangle::initGL()
{
	vertexCount = 4;
	texCoords = genTexCoords();

	programID = LoadShaders("Shaders/tex.vs", "Shaders/tex.fs");
	timeID = glGetUniformLocation(programID, "time");
	texUniform = glGetUniformLocation(programID, "sampler");
	time = 0;

	glUniform1ui(timeID, time);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * 4, vertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &texCoordsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * 4, texCoords, GL_STATIC_DRAW);
}

void Rectangle::attachTexture(GLuint width, GLuint height, void* data)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glUniform1i(texUniform, 0);
}

void Rectangle::attachShaders(const std::string vertexShader, const std::string fragmentShader)
{
	programID = LoadShaders(vertexShader.c_str(), fragmentShader.c_str());
	timeID = glGetUniformLocation(programID, "time");
	texUniform = glGetUniformLocation(programID, "sampler");
}

float* Rectangle::genVertices()
{
	float* verts = new float[8];

	verts[0] = topLeft.x;
	verts[1] = topLeft.y;
	verts[2] = topLeft.x;
	verts[3] = bottomRight.y;
	verts[4] = bottomRight.x;
	verts[5] = topLeft.y;
	verts[6] = bottomRight.x;
	verts[7] = bottomRight.y;
	
	return verts;
}

float* Rectangle::genTexCoords()
{
	float* tc = new float[8];

	tc[0] = 0;
	tc[1] = 1;
	tc[2] = 0;
	tc[3] = 0;
	tc[4] = 1;
	tc[5] = 1;
	tc[6] = 1;
	tc[7] = 0;

	return tc;
}

void Rectangle::UseProgram()
{
	glUseProgram(programID);
}


void Rectangle::SetTime(GLuint time)
{
	this->time = time;
	glUniform1ui(timeID, time);
}

GLuint Rectangle::GetTime()
{
	return time;
}