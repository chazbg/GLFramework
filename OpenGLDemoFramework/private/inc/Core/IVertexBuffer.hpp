#pragma once

class IVertexBuffer
{
public:
    virtual unsigned int getVertexCount() const = 0;
    virtual unsigned int getAttributeSize() const = 0;
protected:
    virtual ~IVertexBuffer() {}
};