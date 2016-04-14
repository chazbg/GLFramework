#include "Core\VertexBuffer.hpp"

#include <GL/glew.h>
VertexBuffer::VertexBuffer() : id(-1), vertexCount(-1), attributeSize(-1)
{

}

VertexBuffer::VertexBuffer(const float * rawData, const int vertexCount, const int attributeSize) : 
	id(-1), vertexCount(vertexCount), attributeSize(attributeSize)
{
	glGenBuffers(1, (GLuint*) &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * attributeSize * sizeof(float), rawData, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, (GLuint*) &id);
}

int VertexBuffer::getId() const
{
	return id;
}

int VertexBuffer::getVertexCount() const
{
	return vertexCount;
}

int VertexBuffer::getAttributeSize() const
{
	return attributeSize;
}
