#pragma once

#include "Core/ITextureCubemap.hpp"

class OpenGLTextureCubemap : public ITextureCubemap
{
    friend class OpenGLResourceManager;
public:
    unsigned int getId() const;
private:
    OpenGLTextureCubemap(const unsigned int id);
    virtual ~OpenGLTextureCubemap();

    const unsigned int id;
};