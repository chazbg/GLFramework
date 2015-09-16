#pragma once

#include <Core/IMaterial.hpp>
#include <string>
#include <vector>
#include <Core/Texture.hpp>

class ShaderMaterial : public IMaterial
{
public:
	ShaderMaterial(const std::string vShaderPath, const std::string fShaderPath);
	~ShaderMaterial();
	virtual int getId() const;
	virtual std::vector<const Texture*> getTextures() const;
	void addTexture(const Texture* tex);
	void removeTexture(const Texture* tex);
private:
	int id;
	std::vector<const Texture*> textures;
};