#include <Core/ShaderMaterial.hpp>
#include <Core/Shader.hpp>

using namespace std;

ShaderMaterial::ShaderMaterial(const std::string vShaderPath, const std::string fShaderPath) : id(-1)
{
	id = LoadShaders(vShaderPath.c_str(), fShaderPath.c_str());
}

ShaderMaterial::~ShaderMaterial()
{
}

int ShaderMaterial::getId() const
{
	return id;
}

std::vector<const Texture*> ShaderMaterial::getTextures() const
{
	return textures;
}

void ShaderMaterial::addTexture(const Texture* tex)
{
	textures.push_back(tex);
}

void ShaderMaterial::removeTexture(const Texture* tex)
{
	vector<const Texture*>::iterator it = find(textures.begin(), textures.end(), tex);
	if (textures.end() != it)
	{
		textures.erase(it);
	}
}