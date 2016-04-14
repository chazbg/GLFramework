#pragma once
#include "Core/IVertexBuffer.hpp"
#include "Core/IIndexBuffer.hpp"
#include "Core/IMaterial.hpp"
#include "Core/Texture.hpp"
#include "Core/TextureCubemap.hpp"

class IResourceManager
{
public:
	virtual ~IResourceManager(){}

	virtual IVertexBuffer* createVertexBufferObject(const float * rawData, const int vertexCount, const int attributeSize) = 0;

	virtual IIndexBuffer* createIndexBufferObject(const unsigned int * rawData, const int indexCount) = 0;

	virtual IMaterial* createMaterial(const std::string vShaderPath, const std::string fShaderPath) = 0;
	
	virtual Texture* createTexture(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char* data) = 0;

	virtual TextureCubemap* createTextureCubemap(
		const Texture* texFront,
		const Texture* texBack,
		const Texture* texTop,
		const Texture* texBottom,
		const Texture* texLeft,
		const Texture* texRight) = 0;
};

