#pragma once

class IIndexBuffer
{
public:
    virtual ~IIndexBuffer() {}
    virtual int getId() const = 0;
    virtual int getIndexCount() const = 0;
};
