#include "Rendering/Variants/OpenGL/OpenGLVertexBuffer.hpp"

OpenGLVertexBuffer::OpenGLVertexBuffer(
    const unsigned int id, 
    const unsigned int location,
    const unsigned int vertexCount,
    const unsigned int attributeSize) :
    id(id), 
    location(location),
    vertexCount(vertexCount),
    attributeSize(attributeSize)    
{

}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
}

unsigned int OpenGLVertexBuffer::getId() const
{
    return id;
}

unsigned int OpenGLVertexBuffer::getLocation() const
{
    return location;
}

unsigned int OpenGLVertexBuffer::getVertexCount() const
{
    return vertexCount;
}

unsigned int OpenGLVertexBuffer::getAttributeSize() const
{
    return attributeSize;
}
