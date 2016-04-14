#pragma once
#include "Core/IIndexBuffer.hpp"

class IndexBuffer: public IIndexBuffer
{
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* rawData, const int indexCount);
    ~IndexBuffer();
    virtual int getId() const;
    virtual int getIndexCount() const;
private:
    int id;
    int indexCount;
};