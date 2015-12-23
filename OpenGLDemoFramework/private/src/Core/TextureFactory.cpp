#include <Core/TextureFactory.hpp>

unsigned int TextureFactory::nextId = 0;

Texture* TextureFactory::createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data)
{
    return new Texture(nextId++, width, height, bpp, data);
}