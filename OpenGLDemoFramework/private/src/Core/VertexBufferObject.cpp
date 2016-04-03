#include "Core/VertexBufferObject.hpp"
#include <cstring>

int VertexBufferObject::nextId = -1;

VertexBufferObject::VertexBufferObject() : id (-1), vertexCount(0), attributeSize(0), data(0)
{

}

VertexBufferObject::VertexBufferObject(const float * rawData, const int vertexCount, const int attributeSize) :
	id(getNextId()), vertexCount(vertexCount), attributeSize(attributeSize), data(0)
{
    if (0 != rawData)
    {
        data = new float[vertexCount * attributeSize];
        memcpy(data, rawData, vertexCount * attributeSize * sizeof(float));
    }
}

VertexBufferObject::~VertexBufferObject()
{
    delete[] data;
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

float * VertexBufferObject::getData() const
{
    return data;
}

int VertexBufferObject::getNextId()
{
    return ++nextId;
}
