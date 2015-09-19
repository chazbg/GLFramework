#pragma once

#include <Core/IMaterial.hpp>
#include <string>
#include <Core/Texture.hpp>

class ShaderMaterial : public IMaterial
{
public:
	ShaderMaterial(const std::string vShaderPath, const std::string fShaderPath);
	~ShaderMaterial();
	virtual int getId() const;
	virtual vector<const Texture*> getTextures() const;
	void addTexture(const Texture* tex);
	void removeTexture(const Texture* tex);
    virtual map<string, int>          getIntProperties() const;
    virtual map<string, unsigned int> getUintProperties() const;
    virtual map<string, float>        getFloatProperties() const;
    virtual map<string, Vec3>         getVec3Properties() const;
    virtual map<string, Matrix4>      getMatrix4Properties() const;
    virtual void setProperty(const string name, const int v);
    virtual void setProperty(const string name, const unsigned int v);
    virtual void setProperty(const string name, const float v);
    virtual void setProperty(const string name, const Vec3& v);
    virtual void setProperty(const string name, const Matrix4& v);
private:
	int id;
	vector<const Texture*> textures;
    map<string, int> iUniforms;
    map<string, unsigned int> uiUniforms;
    map<string, float> fUniforms;
    map<string, Vec3> vUniforms;
    map<string, Matrix4> mUniforms;
};