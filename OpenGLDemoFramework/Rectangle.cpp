#include "Rectangle.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Rectangle::Rectangle() : topLeft(-1, 1), bottomRight(1, -1)
{
	init();
}

Rectangle::Rectangle(Vec2 topLeft, Vec2 bottomRight) : topLeft(topLeft), bottomRight(bottomRight)
{
	init();
}

Rectangle::~Rectangle() 
{
	delete[] texCoords;
}

void Rectangle::Render()
{
	SetTime(time + 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	Mesh::Render();
}

void Rectangle::init()
{
	vertexCount = 6;
	time = 0;
	float* vertexBuffer = genVertices();
	texCoords = genTexCoords();
	SetShaders("Shaders/tex.vs", "Shaders/tex.fs");
	BindUniform("time");
	SetUniformValue("time", time);
	BindUniform("sampler");
	SetVertexBuffer(vertexBuffer, vertexCount, 2);	
	SetTexCoordsBuffer(texCoords);
	glGenTextures(1, &texID);
}

void Rectangle::attachTexture(const Texture& texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getWidth(), texture.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SetUniformValue("sampler", 0);
}

void Rectangle::attachShaders(const std::string vertexShader, const std::string fragmentShader)
{
	SetShaders(vertexShader.c_str(), fragmentShader.c_str());
	BindUniform("time");
	BindUniform("sampler");
}

float* Rectangle::genVertices()
{
	float* verts = new float[12];

	//Top left
	verts[0] = topLeft.x;
	verts[1] = topLeft.y;

	//Bottom left
	verts[2] = topLeft.x;
	verts[3] = bottomRight.y;

	//Top right
	verts[4] = bottomRight.x;
	verts[5] = topLeft.y;

	//Bottom left
	verts[6] = topLeft.x;
	verts[7] = bottomRight.y;
	
	//Bottom right
	verts[8] = bottomRight.x;
	verts[9] = bottomRight.y;

	//Top right
	verts[10] = bottomRight.x;
	verts[11] = topLeft.y;
	return verts;
}

float* Rectangle::genTexCoords()
{
	float* tc = new float[12];

	//Top left
	tc[0] = 0;
	tc[1] = 1;

	//Bottom left
	tc[2] = 0;
	tc[3] = 0;

	//Top right
	tc[4] = 1;
	tc[5] = 1;

	//Bottom left
	tc[6] = 0;
	tc[7] = 0;

	//Bottom right
	tc[8] = 1;
	tc[9] = 0;

	//Top right
	tc[10] = 1;
	tc[11] = 1;

	return tc;
}

void Rectangle::SetTime(GLuint time)
{
	this->time = time;
	SetUniformValue("time", time);
}

GLuint Rectangle::GetTime()
{
	return time;
}