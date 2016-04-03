#pragma once
#include "Core/Mesh.hpp"
#include "Math/Vector.hpp"
#include "Core/Texture.hpp"
#include <Core/VertexBufferObject.hpp>
#include <string>

class RectangleGeometry : public Mesh
{
public:
	RectangleGeometry();
	RectangleGeometry(Vec2 topLeft, Vec2 bottomRight);
	~RectangleGeometry();
private:
	Vec2 topLeft;
	Vec2 bottomRight;
	float* genVertices();
	float* genTexCoords();
	void init();
	unsigned int time;
	VertexBufferObject* vertices;
	VertexBufferObject* texCoords;
};