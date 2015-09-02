#include <Core/ShaderMaterial.hpp>
#include <Core/Shader.hpp>

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
