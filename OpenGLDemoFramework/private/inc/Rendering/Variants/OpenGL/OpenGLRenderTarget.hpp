#pragma once

#include "Core/IRenderTarget.hpp"
#include <vector>

class OpenGLRenderTarget : public IRenderTarget
{
    friend class OpenGLResourceManager;
public:
    virtual void addColorTexture(ITexture* texture);
    virtual ITexture* getColorTexture(const unsigned int index) const;
    virtual void addDepthTexture(ITexture* texture);
    virtual ITexture* getDepthTexture() const;

    unsigned int getId() const;
private:
    OpenGLRenderTarget(const unsigned int id, 
        const unsigned int width, 
        const unsigned int height);
    ~OpenGLRenderTarget();
    void updateRenderTarget();

    const unsigned int id;
    const unsigned int width;
    const unsigned int height;
    std::vector<ITexture*> colorTextures;
    ITexture* depthTexture;
};