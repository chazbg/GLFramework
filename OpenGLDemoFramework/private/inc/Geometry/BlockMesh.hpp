#pragma once

#include "Core/Mesh.hpp"
#include <Core/VertexBufferObject.hpp>

class BlockMesh : public Mesh
{
public:
	BlockMesh(const float width = 1, const float height = 1, const float length = 1);
	~BlockMesh();
	void setTime(const unsigned int time);
	unsigned int getTime();
private:
	float width;
	float height;
	float length;
	float* genVerts();
	unsigned int time;
    VertexBufferObject* vertices;
    VertexBufferObject* normals;
    VertexBufferObject* uvs;
};