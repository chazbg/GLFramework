#pragma once
#include "Core/IImageLoader.hpp"

class TextureLoader : public ITextureLoader
{
public:
	TextureLoader();
	~TextureLoader();
	virtual Texture* loadTexture(const std::string path);
};

