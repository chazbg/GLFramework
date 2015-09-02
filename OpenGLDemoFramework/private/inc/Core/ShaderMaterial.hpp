#pragma once

#include <Core/IMaterial.hpp>
#include <string>

class ShaderMaterial : public IMaterial
{
public:
	ShaderMaterial(const std::string vShaderPath, const std::string fShaderPath);
	~ShaderMaterial();
	virtual int getId() const;
private:
	int id;
};