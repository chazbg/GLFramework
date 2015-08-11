#include "Core/Texture.hpp"
#include <cstring>

Texture::Texture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data) :
width(width),
height(height),
bpp(bpp),
data(0)
{
	if (0 != data)
	{
		this->data = new unsigned char[width * height * bpp];
		memcpy(this->data, data, width * height * bpp);
	}
}

Texture::~Texture()
{
	if (0 != data)
	{
		delete[] data;
	}
}

unsigned int Texture::getWidth() const
{
	return width;
}

unsigned int Texture::getHeight() const
{
	return height;
}

unsigned int Texture::getBpp() const
{
	return bpp;
}

unsigned char* Texture::getData() const
{
	return data;
}