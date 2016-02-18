#include <Core/IndexBufferObject.hpp>

#include <GL/glew.h>

IndexBufferObject::IndexBufferObject() : id(-1), indexCount(0)
{
}

IndexBufferObject::IndexBufferObject(const unsigned int * rawData, const int indexCount) : id(-1), indexCount(indexCount)
{
    glGenBuffers(1, (GLuint*)&id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(float), rawData, GL_STATIC_DRAW);
}

IndexBufferObject::~IndexBufferObject()
{
    glDeleteBuffers(1, (GLuint*)&id);
}

int IndexBufferObject::getId() const
{
    return id;
}

int IndexBufferObject::getIndexCount() const
{
    return indexCount;
}
