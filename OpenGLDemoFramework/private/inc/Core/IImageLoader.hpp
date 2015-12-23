#pragma once

#include <string>
#include <Core/Texture.hpp>

class ITextureLoader
{
public:
	virtual ~ITextureLoader() {}
	virtual Texture* loadTexture(const std::string path) = 0;
};