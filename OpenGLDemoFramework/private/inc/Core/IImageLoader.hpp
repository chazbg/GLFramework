#pragma once

#include <string>
#include <Core/Texture.hpp>
#include <Core/TextureCubemap.hpp>

class ITextureLoader
{
public:
	virtual ~ITextureLoader() {}
	virtual Texture* loadTexture(const std::string path) = 0;
	virtual TextureCubemap* loadTextureCubemap(
		const std::string pathFront,
		const std::string pathBack,
		const std::string pathTop,
		const std::string pathBottom,
		const std::string pathLeft,
		const std::string pathRight) = 0;
};