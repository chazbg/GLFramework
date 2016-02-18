#pragma once
#include "Core/IImageLoader.hpp"

class TextureLoader : public ITextureLoader
{
public:
	TextureLoader();
	~TextureLoader();
	virtual Texture* loadTexture(const std::string path);
	virtual TextureCubemap* loadTextureCubemap(
		const std::string pathFront,
		const std::string pathBack,
		const std::string pathTop,
		const std::string pathBottom,
		const std::string pathLeft,
		const std::string pathRight);
};

