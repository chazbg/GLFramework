#include "Rectangle.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Rectangle::Rectangle()
{
	vertexBuffer[0] = -1;
	vertexBuffer[1] = 1;
	vertexBuffer[2] = 0;
	vertexBuffer[3] = 1;
	vertexBuffer[4] = -1;
	vertexBuffer[5] = -1;
	vertexBuffer[6] = 0;
	vertexBuffer[7] = 0;
	vertexBuffer[8] = 1;
	vertexBuffer[9] = -1;
	vertexBuffer[10] = 1;
	vertexBuffer[11] = 0;
	vertexBuffer[12] = -1;
	vertexBuffer[13] = 1;
	vertexBuffer[14] = 0;
	vertexBuffer[15] = 1;
	vertexBuffer[16] = 1;
	vertexBuffer[17] = -1;
	vertexBuffer[18] = 1;
	vertexBuffer[19] = 0;
	vertexBuffer[20] = 1;
	vertexBuffer[21] = 1;
	vertexBuffer[22] = 1;
	vertexBuffer[23] = 1;

	initGL();
}

Rectangle::Rectangle(Vec2 topLeft, Vec2 bottomRight)
{
	//vertexBuffer[0] = topLeft.x;
	//vertexBuffer[1] = bottomRight.y;
	//vertexBuffer[2] = 0;
	//vertexBuffer[3] = 0;
	//vertexBuffer[4] = bottomRight.x;
	//vertexBuffer[5] = bottomRight.y;
	//vertexBuffer[6] = 1;
	//vertexBuffer[7] = 0;
	//vertexBuffer[8] = topLeft.x;
	//vertexBuffer[9] = topLeft.y;
	//vertexBuffer[10] = 0;
	//vertexBuffer[11] = 1;
	//vertexBuffer[12] = bottomRight.x;
	//vertexBuffer[13] = topLeft.y;
	//vertexBuffer[14] = 1;
	//vertexBuffer[15] = 0;
	//vertexBuffer[16] = topLeft.x;
	//vertexBuffer[17] = topLeft.y;
	//vertexBuffer[18] = 0;
	//vertexBuffer[19] = 1;
	//vertexBuffer[20] = bottomRight.x;
	//vertexBuffer[21] = bottomRight.y;
	//vertexBuffer[22] = 1;
	//vertexBuffer[23] = 1;
	initGL();
}

Rectangle::~Rectangle() {}

void Rectangle::Render()
{
	useProgram();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	time++;
	glUniform1ui(timeUniform, time);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Rectangle::initGL()
{
	float v[] = {
		-1.0f, 1.0f,
		0.0f, 1.0f,
		-1.0f, -1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 0.0f,
	};
	programID = LoadShaders("Shaders/tex.vs", "Shaders/tex.fs");
	timeUniform = glGetUniformLocation(programID, "time");
	texUniform = glGetUniformLocation(programID, "sampler");
	time = 0;

	glUniform1ui(timeUniform, time);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, BUFFER_OFFSET(8));
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