#pragma once
#include <Core/Texture.hpp>
#include <Core/TextureCubemap.hpp>

class TextureFactory
{
public:
    static Texture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data);
	static TextureCubemap* createTextureCubemap(
			const Texture* texFront,
			const Texture* texBack,
			const Texture* texTop,
			const Texture* texBottom,
			const Texture* texLeft,
			const Texture* texRight
		);
private:
	static unsigned int nextId;
};