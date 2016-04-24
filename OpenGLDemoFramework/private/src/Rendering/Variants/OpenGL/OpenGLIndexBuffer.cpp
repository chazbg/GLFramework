#include "Rendering/Variants/OpenGL/OpenGLIndexBuffer.hpp"

OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int id, const unsigned int indexCount) : id(id), indexCount(indexCount)
{
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
}

unsigned int OpenGLIndexBuffer::getId() const
{
    return id;
}

unsigned int OpenGLIndexBuffer::getIndexCount() const
{
    return indexCount;
}
