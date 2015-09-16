#pragma once
#include "Core/Mesh.hpp"
#include "Math/Vector.hpp"
#include "Core/Texture.hpp"
#include <Core/VertexBufferObject.hpp>
#include <string>

class Rectangle : public Mesh
{
public:
	Rectangle();
	Rectangle(Vec2 topLeft, Vec2 bottomRight);
	~Rectangle();
	void SetTime(GLuint time);
	GLuint GetTime();
private:
	Vec2 topLeft;
	Vec2 bottomRight;
	float* genVertices();
	float* genTexCoords();
	void init();
	GLuint texID;
	unsigned int time;
	VertexBufferObject* vertices;
	VertexBufferObject* texCoords;
};