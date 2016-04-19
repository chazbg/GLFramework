#pragma once

class ITexture
{
public:
    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;
    virtual unsigned int getBpp() const = 0;
protected:
    virtual ~ITexture() {}
};