#pragma once
#include <GL/glew.h>
#include "Matrix.hpp"

class Mesh
{
public:
	Mesh();
	virtual void Render() = 0;
	~Mesh();
	void SetViewMatrix(const Matrix4& view);
protected:
	void generateNormals();
	unsigned int vertexCount;
	float* vertexBuffer;
	float* normalsBuffer;
	float* wireframeVertexBuffer;
	Matrix4 view;
};

