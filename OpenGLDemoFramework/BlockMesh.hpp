#pragma once

#include "Mesh.hpp"

class BlockMesh : public Mesh
{
public:
	BlockMesh(const float width = 1, const float height = 1, const float length = 1);
	~BlockMesh();
	virtual void Render();
	virtual void SetShaders(const string vertexShaderPath, const string fragmentShaderPath);
	void setTime(const unsigned int time);
	unsigned int getTime();
private:
	float width;
	float height;
	float length;
	float* genVerts();
	unsigned int time;
};