#pragma once

class IIndexBufferObject
{
public:
    virtual ~IIndexBufferObject() {}
    virtual int getId() const = 0;
    virtual int getIndexCount() const = 0;
};
