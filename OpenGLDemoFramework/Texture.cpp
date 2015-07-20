#include "Texture.hpp"
#include <cstring>

Texture::Texture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data) :
width(width),
height(height),
bpp(bpp)
{
	this->data = new unsigned char[width * height * bpp];
	memcpy(this->data, data, width * height * bpp);
}

Texture::~Texture()
{
	delete[] data;
}