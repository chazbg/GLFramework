#pragma once

#include "Mesh.hpp"

class Triangle : public Mesh
{
public:
	Triangle();
	~Triangle();
	virtual void Render();
	virtual void SetShaders(const string vertexShaderPath, const string fragmentShaderPath);
private:
	float* genVerts();
	unsigned int time;
};