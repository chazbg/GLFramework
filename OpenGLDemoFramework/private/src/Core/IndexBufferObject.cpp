#include "Core/IndexBufferObject.hpp"
#include <cstring>

int IndexBufferObject::nextId = -1;

IndexBufferObject::IndexBufferObject() : id(-1), indexCount(0), data(0)
{
}

IndexBufferObject::IndexBufferObject(const unsigned int * rawData, const int indexCount) : id(-1), indexCount(indexCount)
{
    if (0 != rawData)
    {
        data = new unsigned int[indexCount];
        memcpy(data, rawData, indexCount * sizeof(unsigned int));
    }
}

IndexBufferObject::~IndexBufferObject()
{
    delete[] data;
}

int IndexBufferObject::getId() const
{
    return id;
}

int IndexBufferObject::getIndexCount() const
{
    return indexCount;
}

unsigned int * IndexBufferObject::getData() const
{
    return data;
}

int IndexBufferObject::getNextId()
{
    return ++nextId;
}
