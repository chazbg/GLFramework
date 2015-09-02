#include <Core\VertexBufferObject.hpp>

#include <GL/glew.h>
VertexBufferObject::VertexBufferObject() : id(-1), vertexCount(-1), attributeSize(-1)
{

}

VertexBufferObject::VertexBufferObject(const float * rawData, const int vertexCount, const int attributeSize) : 
	id(-1), vertexCount(vertexCount), attributeSize(attributeSize)
{
	glGenBuffers(1, (GLuint*) &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * attributeSize * sizeof(float), rawData, GL_STATIC_DRAW);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, (GLuint*) &id);
}

int VertexBufferObject::getId() const
{
	return id;
}

int VertexBufferObject::getVertexCount() const
{
	return vertexCount;
}

int VertexBufferObject::getAttributeSize() const
{
	return attributeSize;
}
