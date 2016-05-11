#pragma once

#include "Core/IVertexBuffer.hpp"

class OpenGLVertexBuffer : public IVertexBuffer
{
    friend class OpenGLResourceManager;
public:
    unsigned int getId() const;
    unsigned int getLocation() const;
    virtual unsigned int getVertexCount() const;
    virtual unsigned int getAttributeSize() const;
private:
    OpenGLVertexBuffer(const unsigned int id, const unsigned int location, const unsigned int vertexCount, const unsigned int attributeSize);
    ~OpenGLVertexBuffer();

    unsigned int id;
    unsigned int location;
    unsigned int vertexCount;
    unsigned int attributeSize;
};