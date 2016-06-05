#pragma once

#include "Core/ITexture.hpp"

class IRenderTarget
{
public:
    virtual ~IRenderTarget() {}
    virtual void addColorTexture(ITexture* texture) = 0;
    virtual ITexture* getColorTexture(const unsigned int index) const = 0;
    virtual void addDepthTexture(ITexture* texture) = 0;
    virtual ITexture* getDepthTexture() const = 0;
};