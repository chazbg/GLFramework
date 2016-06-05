#include "Rendering/Variants/OpenGL/OpenGLRenderTarget.hpp"
#include "Rendering/Variants/OpenGL/OpenGLTexture.hpp"
#include <GL/glew.h>
#include <iostream>

OpenGLRenderTarget::OpenGLRenderTarget(const unsigned int id, 
    const unsigned int width,
    const unsigned int height) :
    id(id),
    width(width),
    height(height),
    depthTexture(0)
{

}

OpenGLRenderTarget::~OpenGLRenderTarget()
{
}

void OpenGLRenderTarget::updateRenderTarget()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    for (unsigned int i = 0; i < colorTextures.size(); ++i)
    {
        OpenGLTexture* tex = reinterpret_cast<OpenGLTexture*>(colorTextures[i]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->getId(), 0);
    }

    if (depthTexture)
    {
        OpenGLTexture* tex = reinterpret_cast<OpenGLTexture*>(depthTexture);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->getId(), 0);
    }

    GLenum* drawBuffs = new GLenum[colorTextures.size()];
    for (unsigned int i = 0; i < colorTextures.size(); ++i)
    {
        drawBuffs[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(colorTextures.size(), drawBuffs);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "INVALID FBO: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    delete[] drawBuffs;
}

void OpenGLRenderTarget::addColorTexture(ITexture * texture)
{
    colorTextures.push_back(texture);
    updateRenderTarget();
}

ITexture * OpenGLRenderTarget::getColorTexture(const unsigned int index) const
{
    return colorTextures[index];
}

void OpenGLRenderTarget::addDepthTexture(ITexture * texture)
{
    depthTexture = texture;
    updateRenderTarget();
}

ITexture * OpenGLRenderTarget::getDepthTexture() const
{
    return depthTexture;
}

unsigned int OpenGLRenderTarget::getId() const
{
    return id;
}
