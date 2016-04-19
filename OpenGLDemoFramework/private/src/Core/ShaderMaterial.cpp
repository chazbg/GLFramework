#include "Core/ShaderMaterial.hpp"
#include "Core/Shader.hpp"

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

const std::vector<const ITexture*>& ShaderMaterial::getTextures() const
{
	return textures;
}

const std::vector<const ITextureCubemap*>& ShaderMaterial::getTextureCubemaps() const
{
	return textureCubemaps;
}

void ShaderMaterial::addTexture(const ITexture* tex)
{
	textures.push_back(tex);
}

void ShaderMaterial::addTextureCubemap(const ITextureCubemap* tex)
{
	textureCubemaps.push_back(tex);
}

void ShaderMaterial::removeTexture(const ITexture* tex)
{
	vector<const ITexture*>::iterator it = find(textures.begin(), textures.end(), tex);
	if (textures.end() != it)
	{
		textures.erase(it);
	}
}

void ShaderMaterial::removeTextureCubemap(const ITextureCubemap* tex)
{
	vector<const ITextureCubemap*>::iterator it = find(textureCubemaps.begin(), textureCubemaps.end(), tex);
	if (textureCubemaps.end() != it)
	{
		textureCubemaps.erase(it);
	}
}

const map<string, int>& ShaderMaterial::getIntProperties() const
{
    return iUniforms;
}

const map<string, unsigned int>& ShaderMaterial::getUintProperties() const
{
    return uiUniforms;
}

const map<string, float>& ShaderMaterial::getFloatProperties() const
{
    return fUniforms;
}

const map<string, Vec3>& ShaderMaterial::getVec3Properties() const
{
    return vUniforms;
}

const map<string, Matrix4>& ShaderMaterial::getMatrix4Properties() const
{
    return mUniforms;
}

void ShaderMaterial::setProperty(const string name, const int v)
{
    iUniforms[name] = v;
}

void ShaderMaterial::setProperty(const string name, const unsigned int v)
{
    uiUniforms[name] = v;
}

void ShaderMaterial::setProperty(const string name, const float v)
{
    fUniforms[name] = v;
}

void ShaderMaterial::setProperty(const string name, const Vec3 & v)
{
    vUniforms[name] = v;
}

void ShaderMaterial::setProperty(const string name, const Matrix4 & v)
{
    mUniforms[name] = v;
}
