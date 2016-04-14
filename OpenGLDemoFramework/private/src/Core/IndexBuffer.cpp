#include "Core/IndexBuffer.hpp"

#include "GL/glew.h"

IndexBuffer::IndexBuffer() : id(-1), indexCount(0)
{
}

IndexBuffer::IndexBuffer(const unsigned int * rawData, const int indexCount) : id(-1), indexCount(indexCount)
{
    glGenBuffers(1, (GLuint*)&id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(float), rawData, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, (GLuint*)&id);
}

int IndexBuffer::getId() const
{
    return id;
}

int IndexBuffer::getIndexCount() const
{
    return indexCount;
}
