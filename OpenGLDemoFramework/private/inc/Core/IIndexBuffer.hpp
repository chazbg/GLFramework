#pragma once

class IIndexBuffer
{
public:
    virtual unsigned int getIndexCount() const = 0;
protected:
    virtual ~IIndexBuffer() {}
};
