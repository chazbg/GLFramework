#pragma once
#include "Core/IIndexBufferObject.hpp"

class IndexBufferObject: public IIndexBufferObject
{
public:
    IndexBufferObject();
    IndexBufferObject(const unsigned int* rawData, const int indexCount);
    ~IndexBufferObject();
    virtual int getId() const;
    virtual int getIndexCount() const;
    virtual unsigned int* getData() const;
private:
    static int nextId;
    static int getNextId();

    int id;
    int indexCount;
    unsigned int* data;
};