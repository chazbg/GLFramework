#include "Rectangle.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Rectangle::Rectangle()
{
	vertexBuffer[0] = -1;
	vertexBuffer[1] = 1;
	vertexBuffer[4] = -1;
	vertexBuffer[5] = -1;
	vertexBuffer[8] = 1;
	vertexBuffer[9] = 1;
	vertexBuffer[12] = 1;
	vertexBuffer[13] = -1;

	initGL();
}

Rectangle::Rectangle(Vec2 topLeft, Vec2 bottomRight)
{
	vertexBuffer[0] = topLeft.x;
	vertexBuffer[1] = topLeft.y;
	vertexBuffer[4] = topLeft.x;
	vertexBuffer[5] = bottomRight.y;
	vertexBuffer[8] = bottomRight.x;
	vertexBuffer[9] = topLeft.y;
	vertexBuffer[12] = bottomRight.x;
	vertexBuffer[13] = bottomRight.y;

	initGL();
}

Rectangle::~Rectangle() {}

void Rectangle::Render()
{
	useProgram();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, BUFFER_OFFSET(8));

	time++;
	glUniform1ui(timeUniform, time);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Rectangle::initGL()
{
	vertexBuffer[2] = 0;
	vertexBuffer[3] = 1;

	vertexBuffer[6] = 0;
	vertexBuffer[7] = 0;

	vertexBuffer[10] = 1;
	vertexBuffer[11] = 1;

	vertexBuffer[14] = 1;
	vertexBuffer[15] = 0;

	programID = LoadShaders("Shaders/tex.vs", "Shaders/tex.fs");
	timeUniform = glGetUniformLocation(programID, "time");
	texUniform = glGetUniformLocation(programID, "sampler");
	time = 0;

	glUniform1ui(timeUniform, time);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
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
	timeUniform = glGetUniformLocation(programID, "time");
	texUniform = glGetUniformLocation(programID, "sampler");
}

void Rectangle::useProgram()
{
	glUseProgram(programID);
}