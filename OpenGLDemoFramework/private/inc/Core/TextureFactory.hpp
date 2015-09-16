#pragma once
#include <Core/Texture.hpp>

class TextureFactory
{
public:
	TextureFactory() : nextId(0) {}
	Texture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data)
	{
		return new Texture(nextId++, width, height, bpp, data);
	}
private:
	unsigned int nextId;
};