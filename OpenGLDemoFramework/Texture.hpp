#pragma once

class Texture
{
public:
	Texture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data);
	~Texture();
private:
	const unsigned int width;
	const unsigned int height;
	const unsigned int bpp;
	unsigned char* data;
};