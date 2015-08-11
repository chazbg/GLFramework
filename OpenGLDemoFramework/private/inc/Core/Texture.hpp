#pragma once

class Texture
{
public:
	Texture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data);
	~Texture();
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getBpp() const;
	unsigned char* getData() const;
private:
	const unsigned int width;
	const unsigned int height;
	const unsigned int bpp;
	unsigned char* data;
};