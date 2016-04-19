#pragma once
#include "Core/Mesh.hpp"
#include "Math/Vector.hpp"
#include <Core/VertexBuffer.hpp>
#include <string>

class Rectangle : public Mesh
{
public:
	Rectangle();
	Rectangle(Vec2 topLeft, Vec2 bottomRight);
	~Rectangle();
private:
	Vec2 topLeft;
	Vec2 bottomRight;
	float* genVertices();
	float* genTexCoords();
	void init();
	unsigned int time;
	VertexBuffer* vertices;
	VertexBuffer* texCoords;
};