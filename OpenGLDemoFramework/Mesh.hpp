#pragma once
#include <GL/glew.h>

class Mesh
{
public:
	Mesh();
	virtual void Render() = 0;
	~Mesh();
protected:
	void generateNormals();
	unsigned int vertexCount;
	float* vertexBuffer;
	float* normalsBuffer;
	float* wireframeVertexBuffer;
};

