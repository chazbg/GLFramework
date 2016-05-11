#include "Rendering/Variants/OpenGL/OpenGLTextureCubemap.hpp"

OpenGLTextureCubemap::OpenGLTextureCubemap(const unsigned int id) :
id(id)
{
    
}

OpenGLTextureCubemap::~OpenGLTextureCubemap()
{

}

unsigned int OpenGLTextureCubemap::getId() const
{
    return id;
}
