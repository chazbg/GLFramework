#pragma once
#include <Core/Texture.hpp>

class TextureFactory
{
public:
    TextureFactory();
    Texture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data);
private:
	static unsigned int nextId;
};