#include "Geometry/Rectangle.hpp"
#include "Core/Shader.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Rectangle::Rectangle() : topLeft(-1, 1), bottomRight(1, -1), vertices(0), texCoords(0)
{
	init();
}

Rectangle::Rectangle(Vec2 topLeft, Vec2 bottomRight) : topLeft(topLeft), bottomRight(bottomRight)
{
	init();
}

Rectangle::~Rectangle() 
{
	delete vertices;
	delete texCoords;
}

void Rectangle::init()
{
	vertexCount = 6;
	time = 0;
	
	float* vertexBuffer = genVertices();
	float* uvs = genTexCoords();

	vertices = new VertexBufferObject(vertexBuffer, vertexCount, 2);
	texCoords = new VertexBufferObject(uvs, vertexCount, 2);

	setVertices(*vertices);
	setTexCoords(*texCoords);

	delete[] vertexBuffer;
	delete[] uvs;

	glGenTextures(1, &texID);
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