#pragma once
#include "Core/IIndexBuffer.hpp"

class OpenGLIndexBuffer: public IIndexBuffer
{
    friend class OpenGLResourceManager;
public:
    unsigned int getId() const;
    virtual unsigned int getIndexCount() const;
private:
    OpenGLIndexBuffer(const unsigned int id, const unsigned int indexCount);
    ~OpenGLIndexBuffer();

    unsigned int id;
    unsigned int indexCount;
};